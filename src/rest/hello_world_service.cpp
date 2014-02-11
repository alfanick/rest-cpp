#include "service.h"
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    void before() {
      response->use_json();
    }

    void create() {
      response->data["foo"] = "bar";
    }

    void read() {
      response->data["message"] = "Hello " + (request->parameters["name"].empty() ? "someone" : request->parameters["name"]) + "!";
    }
  private:
    static REST::ServiceRegister<HelloWorldService> reg;
};

REST::ServiceRegister<HelloWorldService> HelloWorldService::reg("HelloWorldService");

