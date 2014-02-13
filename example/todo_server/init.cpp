#include <rest/rest.h>

#include "resources/hello_world_service.cpp"

create_json_service(adder) {
  int a = service->request->parameter("a", 0);
  int b = service->request->parameter("b", 0);

  service->response->data["result"] = a+b;
}

void routes(REST::Router* r) {
  r->resource<HelloWorldService>("przywitanie");

  r->path("sum/:a/:b", adder);

  r->path("fibonacci/:fib", [](REST::Service* service) {
    int num = service->request->parameter("fib", 0);
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
