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

}
