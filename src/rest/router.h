#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <iostream>
#include <map>
#include "service.h"

namespace REST {
typedef std::map<std::string, Service *> names_services_map;
typedef std::map<int, names_services_map *> workers_services_map;

/**
 * Router resolves URL to Request::parameters and
 * finds Service corresponding to the URL.
 *
 * @see Request
 * @see Service
 */
class Router {
  public:
    static Router* Instance();
    static Service* getResource(std::string const &, int);
  private:
    Router() {};
    Router(Router const&) {};
    static Router* pInstance;
    static workers_services_map* workers_services;

};

}

#endif
