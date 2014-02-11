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
    if (path.size() == 0)
      return NULL;

    //TODO: dzialajacy routing
    std::string name = path.substr(1);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    if(name.find("/") != std::string::npos)
      name = name.substr(name.find("/"));
    std::cout << "name: " << name << std::endl;

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

  // void Router::registerPath(std::string const &path, std::function<void(void)>* fun) {
  // }

}
