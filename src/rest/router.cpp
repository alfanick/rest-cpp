#include "router.h"
#include "service.h"
#include "lambda_service.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace REST {
  workers_services_vector Router::workers_services;
  Router* Router::pInstance = NULL;
  lambda_patterns* Router::patterns = new lambda_patterns();
  int Router::WORKERS = 256;

  Router::Router() {
    root = std::make_shared<Router::Node>();

    workers_services.resize(WORKERS);

    for (int i = 0; i < WORKERS; i++)
      workers_services[i] = std::make_shared<names_services_map>();
  }

  Router* Router::Instance() {
    if(pInstance == NULL){
      pInstance = new Router();
    }
    return pInstance;
  }


  std::shared_ptr<Service> Router::getResource(std::shared_ptr<Request> request, int worker_id) {
    std::string path = request->path;
    path_tuple* pair = extractParams(path);
    if(pair == NULL)
      return NULL;

    std::string name = pair->first;
    std::shared_ptr<params_map> params = pair->second;

    // std::cout << "name: " << name << "\nparams:\n";
    // for(params_map::iterator iter = params->begin(); iter != params->end(); ++iter) {
    //   std::cout << "\t" << iter->first << ": " << iter->second << "\n";
    // }

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    std::shared_ptr<names_services_map> names_services = workers_services[worker_id];

    std::shared_ptr<Service> service;
    names_services_map::iterator niter = names_services->find(name);
    if(niter == names_services->end()) {
      service = (std::shared_ptr<Service>) ServiceFactory::createInstance(name);
      if(service == NULL) {
        auto liter = patterns->find(name);
        if(liter != patterns->end())
          service = liter->second.second;
      }
      names_services->insert(std::make_pair(name, service));
    } else {
      service = niter->second;
    }
    request->parameters.insert(params->begin(), params->end());

    return service;
  }


  path_tuple* Router::extractParams(std::string const& path) {
    if(path.size() == 0 || path[0] != '/')
      return NULL;

    std::string str = path.substr(1);
    std::string res_name = str;
    std::string params_part = "";

    if(str.find("/") != std::string::npos) {
      res_name = str.substr(0, str.find("/"));
      params_part = str.substr(str.find("/"));
    }

    std::shared_ptr<params_map> params = std::make_shared<params_map>();

    if(ServiceFactory::exist(res_name) && params_part.size() != 0) {
      int slash = params_part.find("/");
      int number = 0;
      while(slash != std::string::npos) {
        params_part = params_part.substr(slash+1);
        slash = params_part.find("/");
        params->insert(std::make_pair(std::to_string(number++),Request::uri_decode(params_part.substr(0, slash))));
      }
    } else {
      auto iter = patterns->find(res_name);
      if(iter != patterns->end()) {
        std::string pattern = iter->second.first;
        pattern = pattern.substr(pattern.find(res_name) + res_name.size());
        int pattern_separator = pattern.find("/:");
        int param_separator = params_part.find("/");
        while(pattern_separator != std::string::npos && param_separator != std::string::npos) {
          pattern = pattern.substr(pattern_separator+2);
          params_part = params_part.substr(param_separator+1);
          pattern_separator = pattern.find("/:");
          param_separator = params_part.find("/");
          std::string parameter_name = pattern.substr(0, pattern_separator);
          std::string parameter_value = params_part.substr(0, param_separator);
          params->insert(std::make_pair(parameter_name, parameter_value));
        }
      }
    }
    return new path_tuple(res_name, params);
  }

  void Router::path(std::string const& path, service_lambda lambda) {
    if(path.size() == 0)
      return;
    std::string name = path.substr(0,path.find("/"));
    std::shared_ptr<LambdaService> lambda_service = std::make_shared<LambdaService>(lambda);
    patterns->insert(std::make_pair(name, std::make_pair(path, lambda_service)));
  }

  void Router::route(std::string path) {
    root->merge(Router::Node::from_path(path));
  }


  Router::Node::Node(std::string p, std::shared_ptr<Node> const& pr) : path(p), parent(pr) {
  }

  Router::Node::~Node() {
    children.clear();
  }

  std::string Router::Node::uri() {
    std::string address;
    std::shared_ptr<Node> previous = shared_from_this();

    while (previous != nullptr) {
      address = (previous->is_root() ? "" : "/" + previous->path) + address;
      previous = previous->parent;
    }
    return address;
  }

  bool Router::Node::is_last() {
    return children.empty();
  }

  std::shared_ptr<Router::Node> Router::Node::next() {
    return *(children.begin());
  }

  void Router::Node::print(int level) {
    std::cout << std::string(level*2, ' ') << "/" << path << std::endl;
    for (auto next : children)
      next->print(level+1);
  }

  bool Router::Node::merge(std::shared_ptr<Router::Node> const path) {
    if (Router::Node::equal(path, shared_from_this())) {
      std::vector< std::shared_ptr<Node> > common_paths(path->children.size());
      auto irfit = std::set_intersection(path->children.begin(), path->children.end(), children.begin(), children.end(), common_paths.begin(), Router::Node::less);
      common_paths.resize(irfit - common_paths.begin());

      for (auto next : common_paths) {
        auto common = std::find_if(children.begin(), children.end(), [&next](const std::shared_ptr<Node> c) { return Router::Node::equal(next, c); });

        (*common)->merge(next);
      }

      // see whats new to add
      //   - set difference
      std::vector< std::shared_ptr<Node> > new_paths(path->children.size());
      auto difit = std::set_difference(path->children.begin(), path->children.end(), children.begin(), children.end(), new_paths.begin(), Router::Node::less);
      new_paths.resize(difit - new_paths.begin());

      children.insert(new_paths.begin(), new_paths.end());
    }

    return true;
  }

  void Router::match(std::string const& path, params_map& params) {
    root->unify(root, path, params);
  }

  bool Router::Node::is_splat() {
    return path[0] == '*';
  }

  bool Router::Node::is_root() {
    return parent == nullptr;
  }

  bool Router::Node::unify(std::shared_ptr<Router::Node> const& root, std::string const& path, params_map& params) {
    std::shared_ptr<Node> match = unify(root, from_path(path), params);

    if (match != nullptr) {
      std::cout << "matched '"<< path <<"' to '"<<match->uri()<<"'\n";
    }

    return match != nullptr;
  }

  std::shared_ptr<Router::Node> Router::Node::unify(std::shared_ptr<Router::Node> const& root, std::shared_ptr<Router::Node> const path, params_map& params) {
    if (Router::Node::unifiable(root, path)) {
      if ((root->is_last() && (path->is_last() || root->is_splat())) ||
          (path->is_root() && path->is_last())) {
        return root;
      } else
      if (root->is_last() && !path->is_last()) {
        return nullptr;
      }
    }

    std::shared_ptr<Node> match;

    for (auto next : root->children) {
      if (!Router::Node::unifiable(next, path->next()))
        continue;

      if ((match = unify(next, path->next(), params)) != nullptr) {
        next->inject(path->next(), params);
        return match;
      }
    }

    return nullptr;
  }

  void Router::Node::inject(std::shared_ptr<Router::Node> const& rhs, params_map& params) {
    if (path[0] == ':') {
      params[path.substr(1)] = rhs->path;
    } else
    if (path[0] == '*') {
      int i = 1;
      std::string splat;
      std::shared_ptr<Node> current = rhs;
      while (true) {
        splat += current->path;
        params[std::to_string(i++)] = current->path;
        if (!current->is_last()) {
          splat += "/";
        } else {
          break;
        }
        current = current->next();
      };
      params["0"] = splat;
    }
  }

  std::shared_ptr<Router::Node> Router::Node::from_path(std::string const& p) {
    std::string path = p;

    std::shared_ptr<Node> root = std::make_shared<Node>();
    std::shared_ptr<Node> current = root;

    while (path.find("/") == 0) {
      path.erase(0, 1);
      if (path.empty())
        break;

      size_t next = path.find("/");

      std::string name = path;

      if (next != std::string::npos) {
        name = path.substr(0, next);
        path = path.substr(next);
      }

      std::shared_ptr<Node> node = std::make_shared<Node>(name, current);
      current->children.insert(node);
      current = node;
    }

    return root;
  }
}
