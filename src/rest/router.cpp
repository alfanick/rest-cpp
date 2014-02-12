#include "router.h"
#include "service.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace REST {

  workers_services_vector Router::workers_services;
  Router* Router::pInstance = NULL;
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

  std::shared_ptr<Service> Router::getResource(std::string const& path, int worker_id) {
    path_tuple* pair = extractParams(path);
    if(pair == NULL)
      return NULL;

    std::string name = pair->first;
    std::shared_ptr<params_map> params = pair->second;
    
    std::cout << "name: " << name << "\nparams:\n";
    for(params_map::iterator iter = params->begin(); iter != params->end(); ++iter) {
      std::cout << "\t" << iter->first << ": " << iter->second << "\n";
    }

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    std::shared_ptr<names_services_map> names_services = workers_services[worker_id];

    std::shared_ptr<Service> service;
    names_services_map::iterator niter = names_services->find(name);
    if(niter == names_services->end()) {
      service = (std::shared_ptr<Service>) ServiceFactory::createInstance(name);
      names_services->insert(std::make_pair(name, service));
    } else {
      service = niter->second;
    }
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
        params->insert(std::make_pair(std::to_string(number++),params_part.substr(0, slash)));
      }
    } else {
    
    }
    
    std::cout << "name: " << res_name << "\nparams: " << params_part << std::endl;
    return new path_tuple(res_name, params);
  }


  // void Router::registerPath(std::string const &path, std::function<void(void)>* fun) {
  // }

}
