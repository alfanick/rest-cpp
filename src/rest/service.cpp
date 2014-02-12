#include "service.h"

namespace REST {

  services_map* ServiceFactory::services = NULL;

  std::shared_ptr<Service> ServiceFactory::createInstance(std::string const &name) {
    services_map::iterator iter= Map()->find(name);
    if(iter == Map()->end())
      return NULL;
    return iter->second();
  }

  services_map* ServiceFactory::Map() {
    if(services == NULL)
      services = new services_map;
    return services;
  }

  bool ServiceFactory::exist(std::string const& name) {
    services_map::iterator iter = Map()->find(name);
    if(iter == Map()->end())
      return false;
    return true;
  }


  Service::Service() {}

  void Service::before() {}
  void Service::after() {}
  void Service::read() {
    throw HTTP::MethodNotAllowed();
  }
  void Service::create() {
    throw HTTP::MethodNotAllowed();
  }
  void Service::update() {
    throw HTTP::MethodNotAllowed();
  }
  void Service::destroy() {
    throw HTTP::MethodNotAllowed();
  }

}
