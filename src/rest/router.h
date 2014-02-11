#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <iostream>
#include <map>
#include <functional>
#include "service.h"

namespace REST {
typedef std::map<std::string, std::shared_ptr<Service> > names_services_map;
typedef std::map<int, std::shared_ptr<names_services_map> > workers_services_map;

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
    static std::shared_ptr<Service> getResource(std::string const &, int);
    // static void registerPath(std::string const &, std::function<void(void)> *);
  private:
    Router() {};
    Router(Router const&) {};
    static Router* pInstance;
    static std::shared_ptr<workers_services_map> workers_services;

};

}

#endif
