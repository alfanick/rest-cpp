#include "service.h"

namespace REST {

  services_map* ServiceFactory::services = NULL;

  Service* ServiceFactory::createInstance(string const &name) {
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

  Service::Service() {}

}
