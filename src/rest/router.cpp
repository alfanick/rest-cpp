#include "router.h"
#include "service.h"
#include <string>
#include <iostream>

namespace REST {

  Router* Router::pInstance = NULL;

  Router* Router::Instance() {
    if(pInstance == NULL)
      pInstance = new Router;
    return pInstance;
  }

  Service* Router::getResource(string const& path) {
    string name = path.substr(1);
    if(name.find("/") != std::string::npos)
      name = name.substr(name.find("/"));
    cout << "name: " << name << std::endl;
    return ServiceFactory::createInstance(name);
  }

}
