#include "lambda_service.h"

namespace REST {

  LambdaService::LambdaService() {}
  LambdaService::LambdaService(LambdaService::function lambda) {
    fun = lambda;
  }

  void LambdaService::method(Request::Method method) {
    fun(this);
  }

}
