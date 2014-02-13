#include <rest/service.h>
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    HelloWorldService() {
      std::cout << "nowy HelloWorldService\n";
    }

    void before() {
      response->use_json();
    }

    void create() {
      response->data["foo"] = "bar";
    }

    void read() {
      response->data["message"] = "Hello " + (request->parameters["0"].empty() ? "someone" : request->parameters["0"]) + "!";
    }
  private:
    static REST::ServiceRegister<HelloWorldService> reg;
};

REST::ServiceRegister<HelloWorldService> HelloWorldService::reg("przywitanie");

