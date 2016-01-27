#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <set>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
#include <tuple>
#include <regex>
#include "service.h"
#include "lambda_service.h"
#include "worker.h"

namespace REST {

/**
 * Router resolves URL to Request::parameters and
 * finds Service corresponding to the URL.
 *
 * @see Request
 * @see Service
 */
class Router {
  public:
    inline static Router* instance() {
      static Router router;
      return &router;
    }

    static Service::shared find(Request::shared, int);

    void match(std::string const &, LambdaService::function);

    template <class R>
    void mount(std::string const& path, bool exact) {
      auto r = to_regex(path, exact);

      std::vector<Service::shared> services;
      services.resize(Worker::POOL_SIZE);
      for (int i = 0; i < Worker::POOL_SIZE; i++)
        services[i] = std::make_shared<R>();

      routes.push_back(std::make_tuple(r, path, services));
    }

    template <class R>
    void mount(std::string const& path) {
      mount<R>(path, false);
    }

    template <class R>
    void resource(std::string const& path) {
      mount<R>(path, true);
    }

    template <class R>
    void resources(std::string const& path) {
      mount<R>(path);
    }

    template <class R, int N>
    void resources() {
      resources<R>(to_path<R, N>());
    }

    template <class R>
    void resources() {
      resources<R, 0>();
    }

    template <class R, int N>
    void resource() {
      resource<R>(to_path<R, N>());
    }

    template <class R>
    void resource() {
      resource<R, 0>();
    }

    void print();

    ~Router();

  private:
    std::pair<std::regex, std::vector<std::string>> to_regex(std::string path, bool exact);

    template <class R, int N>
    static std::string to_path() {
      std::string name = typeid(R).name();
      std::string path = "";
      path.reserve(name.size());
      std::transform(name.begin(), name.end(), name.begin(), ::tolower);
      size_t number_position = 0;
      int strip = N;
      while ((number_position = name.find_first_of("0123456789")) != std::string::npos) {
        if (number_position != 0)
          name = name.substr(number_position);
        std::string number;

        for (size_t next_digit = 0; ::isdigit(name[next_digit]); next_digit++)
          number += name[next_digit];

        size_t name_length = std::stol(number);

        name = name.substr(number.size());
        if (strip <= 0)
          path += "/"+name.substr(0, name_length);
        else
          strip--;
        name = name.substr(name_length);
      }

      auto s = path.rfind("service");
      if (s == (path.size() - 7))
        path.erase(path.rfind("service"));

      s = path.rfind("resource");
      if (s == (path.size() - 8))
        path.erase(path.rfind("resource"));

      while (strip < 0) {
        strip++;

        //std::cout << path << std::endl;
        path.erase(path.rfind("/"));
      }

      return path;
    }

    Router();

    std::vector<std::tuple<std::pair<std::regex, std::vector<std::string>>, std::string, std::vector<Service::shared>>> routes;

};

}

#endif
