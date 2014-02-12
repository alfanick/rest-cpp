#ifndef REST_CPP_MIDDLEWARE_H
#define REST_CPP_MIDDLEWARE_H

#include <vector>

namespace REST {

  /**
   * Each Request object is proceeded by
   * stack of Middlewares before it gets
   * to Service object
   *
   * @see Request
   */
  class Middleware {
  };

}

#endif
