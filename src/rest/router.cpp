#include "router.h"
#include "service.h"
#include "resource.h"
#include "lambda_service.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace REST {
  Router::Router() {
  }

  Router::~Router() {
    // delete root;
  }

  void Router::print() {
    std::cout << "Available routes:\n";

    for (auto& r : routes) {
      std::cout << "  " << std::get<1>(r);

      auto service = std::get<2>(r)[0];

      if (dynamic_cast< LambdaService* >(service.get()) != nullptr) {
        std::cout << " - lambda";
      } else
      if (dynamic_cast< Resource* >(service.get()) != nullptr) {
        std::cout << " - resource";
      } else {
        std::cout << " - service";
      }

      if (service->features.size() > 0) {
        std::cout << " (";
        int s = service->features.size();

        for (auto feature : service->features) {
          std::cout << feature->feature_name();
          if (--s)
            std::cout << ", ";
        }
        std::cout << ")";
      }

      std::cout << std::endl;
    }
  }

  Router* Router::instance() {
    static Router router;
    return &router;
  }

  std::pair<std::regex, std::vector<std::string>> Router::to_regex(std::string path, bool exact) {
    std::vector<std::string> params;
    path = ((!path.empty()) && (path[0] != '/')) ? "/" + path : path;

    std::string regex = "^";
    regex.reserve(path.size());

    size_t start = 1;

    while (true) {
      regex += "/";
      size_t next = path.find("/", start);

      std::string section = path.substr(start, next - start);
      std::string format;

      if (next >= path.size()+1) {
        size_t dot = section.find(".");

        if (dot != std::string::npos) {
          format = section.substr(dot);
          section = section.substr(0, dot);
        }
      }

      if (section[0] == ':') {
        regex += "([^/]+)";
        params.emplace_back(section.substr(1));
      } else
      if (section[0] == '*') {
        regex += "(.+)";
        params.emplace_back(section.size() == 1 ? "*" : section.substr(1));
      } else {
        regex += section;
      }

      if (next >= path.size()) {
        if (!format.empty())
          regex += "\\" + format;
        break;
      }

      start = next + 1;
    }

    if (exact) {
      regex += "$";
    } else {
      regex += "/?(.*)$";
      params.emplace_back("*");
    }

    return std::make_pair(std::regex(regex), params);
  }

  Service::shared Router::find(Request::shared request, int worker_id) {
    std::string &path = request->path;
    std::smatch matches;

    for (auto& route : instance()->routes) {
      std::regex_search(path, matches, std::get<0>(route).first);

      if (!matches.empty()) {
        auto &params = std::get<0>(route).second;

        for (size_t i = 1; i < matches.size(); i++) {
          request->parameters[params[i-1]] = matches[i];
          request->parameters[std::to_string(i)] = matches[i];
        }

        return std::get<2>(route)[worker_id];
      }
    }

    return nullptr;
  }
  void Router::match(std::string const& path, LambdaService::function lambda) {
    auto r = to_regex(path, false);

    std::vector<Service::shared> services;
    services.resize(Worker::POOL_SIZE);
    for (int i = 0; i < Worker::POOL_SIZE; i++)
      services[i] = std::make_shared<LambdaService>(lambda);

    routes.push_back(std::make_tuple(r, path, services));
  }

}
