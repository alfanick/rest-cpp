#ifndef REST_CPP_LAMBDA_SERVICE_H
#define REST_CPP_LAMBDA_SERVICE_H

#include <functional>
#include <iostream>
#include "service.h"


namespace REST {

typedef std::function<void(Service *)> service_lambda;

/**
 * @private
 */
class LambdaService : public Service {
  public:
    LambdaService();
    LambdaService(std::shared_ptr<LambdaService> const& ls);
    LambdaService(service_lambda);
  protected:
    void make_action() final;
  private:
    service_lambda fun;
};

}

#endif
