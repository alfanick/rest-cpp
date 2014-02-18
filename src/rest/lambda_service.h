#ifndef REST_CPP_LAMBDA_SERVICE_H
#define REST_CPP_LAMBDA_SERVICE_H

#include <functional>
#include <iostream>
#include "service.h"


namespace REST {

/**
 * @private
 */
class LambdaService : public Service {
  public:
    typedef std::function<void(LambdaService *)> function;

    LambdaService();
    LambdaService(std::shared_ptr<LambdaService> const& ls);
    LambdaService(function);
  protected:
    void make_action() final;
  private:
    function fun;
};

}

#endif
