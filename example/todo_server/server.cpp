#include <rest/server.h>

#include <iostream>
#include <signal.h>
#include <cstdlib>

REST::Server *server;

void stop_server(int a = 0) {
  delete server;
}

int main(int argc, char **argv) {
  signal(SIGINT, stop_server);

  server = new REST::Server("0.0.0.0", 8080, (argc > 1) ? atoi(argv[1]) : 4);

  server->router()->registerPath("sum/:num1/:num2", [](REST::Service* service) {
    int num1 = (!service->request->parameters["num1"].empty()) ? std::stoi(service->request->parameters["num1"]) : 0;
    int num2 = (!service->request->parameters["num2"].empty()) ? std::stoi(service->request->parameters["num2"]) : 0;
    int res = num1+num2;
    service->response->use_json();
    service->response->data["result"] = res;
  });

  server->router()->registerPath("fibonacci/:fib", [](REST::Service* service) {
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

  server->run();

  return 0;
}
