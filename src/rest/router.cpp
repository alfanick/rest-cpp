#include "router.h"
#include "service.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace REST {

  Router* Router::pInstance = NULL;

  Router* Router::Instance() {
    if(pInstance == NULL)
      pInstance = new Router;
    return pInstance;
  }

  Service* Router::getResource(std::string const& path) {
    if (path.size() == 0)
      return NULL;

    std::string name = path.substr(1);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    if(name.find("/") != std::string::npos)
      name = name.substr(name.find("/"));
    std::cout << "name: " << name << std::endl;
    return ServiceFactory::createInstance(name);
  }

}
