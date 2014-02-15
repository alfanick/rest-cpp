#include "lambda_service.h"

namespace REST {

  LambdaService::LambdaService(std::shared_ptr<LambdaService> const& ls) {
    fun = ls->fun;
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
