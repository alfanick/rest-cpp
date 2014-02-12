#include "lambda_service.h"

namespace REST {

  LambdaService::LambdaService(LambdaService &ls) {
    fun = ls.fun;
  }

  LambdaService::LambdaService() {}
  LambdaService::LambdaService(service_lambda lambda) {
    fun = lambda;
  }

  LambdaService::~LambdaService() {}

  void LambdaService::make_action() {
    fun(this);
  }

}
