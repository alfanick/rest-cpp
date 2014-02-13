#include <rest/rest.h>

#include "hello_world_service.cpp"

void prepare(REST::Server* s, REST::Router* r) {
  s->resource<HelloWorldService>("przywitanie");

  r->registerPath("sum/:num1/:num2", [](REST::Service* service) {
    int num1 = (!service->request->parameters["num1"].empty()) ? std::stoi(service->request->parameters["num1"]) : 0;
    int num2 = (!service->request->parameters["num2"].empty()) ? std::stoi(service->request->parameters["num2"]) : 0;
    int res = num1+num2;
    service->response->use_json();
    service->response->data["result"] = res;
  });

  r->registerPath("fibonacci/:fib", [](REST::Service* service) {
    int num = std::stoi(service->request->parameters["fib"]);
    int fib1=1, fib2=1, res=0;
    if(num ==0 || num == 1 || num == 2)
      res = 1;
    else {
      num -= 2;
      while(num--) {
        res = fib1 + fib2;
        fib1 = fib2;
        fib2 = res;
      }
    }
    service->response->use_json();
    service->response->data["result"] = res;
  });
}
