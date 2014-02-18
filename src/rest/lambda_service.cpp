#include "lambda_service.h"

namespace REST {

  LambdaService::LambdaService(std::shared_ptr<LambdaService> const& ls) {
    fun = ls->fun;
  }

  LambdaService::LambdaService() {}
  LambdaService::LambdaService(LambdaService::function lambda) {
    fun = lambda;
  }

  void LambdaService::make_action() {
    fun(this);
  }

}
