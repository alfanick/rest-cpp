#include "router.h"

namespace REST {

  Router* Router::pInstance = NULL;

  Router* Router::Instance() {
    if(pInstance == NULL)
      pInstance = new Router;
    return pInstance;
  }

}
