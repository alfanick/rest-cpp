#include "router.h"
#include "service.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace REST {

  workers_services_map* Router::workers_services = new workers_services_map();
  Router* Router::pInstance = NULL;

  Router* Router::Instance() {
    if(pInstance == NULL)
      pInstance = new Router;
    return pInstance;
  }

  Service* Router::getResource(std::string const& path, int worker_id) {
    if (path.size() == 0)
      return NULL;

    std::string name = path.substr(1);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    if(name.find("/") != std::string::npos)
      name = name.substr(name.find("/"));
    std::cout << "name: " << name << std::endl;

    workers_services_map::iterator witer = workers_services->find(worker_id);
    names_services_map* names_services;
    if(witer == workers_services->end()) {
      names_services = new names_services_map();
      workers_services->insert(std::make_pair(worker_id,names_services));
    } else {
      names_services = witer->second;
    }

    Service* service;
    names_services_map::iterator niter = names_services->find(name);
    if(niter == names_services->end()) {
      service = ServiceFactory::createInstance(name);
      names_services->insert(std::make_pair(name, service));
    } else {
      service = niter->second;
    }

    return service;
  }

}
