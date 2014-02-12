#ifndef REST_CPP_LAMBDA_SERVICE_H
#define REST_CPP_LAMBDA_SERVICE_H

#include <functional>
#include <iostream>
#include "service.h"


namespace REST {

typedef std::function<void(Service *)> service_lambda;

class LambdaService : public Service { 
  public:
    LambdaService();
    LambdaService(LambdaService &ls);
    LambdaService(service_lambda);
    ~LambdaService();
    void make_action();
  private:
    service_lambda fun;
};

}

#endif
