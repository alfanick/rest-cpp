#ifndef REST_CPP_LAMBDA_SERVICE_H
#define REST_CPP_LAMBDA_SERVICE_H

#include <functional>
#include <iostream>
#include "service.h"
#include "features/authorization.h"

namespace REST {

/**
 * @private
 */
class LambdaService : public virtual Service,
  public Features::Authorization {
  public:
    typedef std::function<void(LambdaService *)> function;

    LambdaService();
    LambdaService(std::shared_ptr<LambdaService> const& ls);
    LambdaService(function);
  protected:
    void method(Request::Method m) final;
  private:
    function fun;
};

}

#endif
