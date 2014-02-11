#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <iostream>

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
  private:
    Router() {};
    Router(Router const&) {};
    static Router* pInstance;

};

}

#endif
