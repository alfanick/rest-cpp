#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <iostream>
#include "service.h"

namespace REST {

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
    static Service* getResource(std::string const &);
  private:
    Router() {};
    Router(Router const&) {};
    static Router* pInstance;

};

}

#endif
