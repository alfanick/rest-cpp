#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <iostream>
#include <map>
#include <functional>
#include <vector>
#include "service.h"

namespace REST {
typedef std::map<std::string, std::shared_ptr<Service> > names_services_map;
typedef std::vector< std::shared_ptr<names_services_map> > workers_services_vector;
typedef std::map<std::string, std::string> params_map;
typedef std::pair<std::string, std::shared_ptr<params_map> > path_tuple;

/**
 * Router resolves URL to Request::parameters and
 * finds Service corresponding to the URL.
 *
 * @see Request
 * @see Service
 */
class Router {
  public:
    static int WORKERS;
    static Router* Instance();

    static std::shared_ptr<Service> getResource(std::string const &, int);
    // static void registerPath(std::string const &, std::function<void(void)> *);
  private:
    Router();
    static Router* pInstance;
    static workers_services_vector workers_services;
    static path_tuple* extractParams(std::string const&);
};

}

#endif
