#include "service.h"
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    void create() {
      response->use_json();

      response->data["foo"] = "bar";
    }

    void read() {
      response->raw = "{ message: \"Hello "+request->parameters["name"]+"!\" }";
    }
  private:
    static REST::ServiceRegister<HelloWorldService> reg;
};

REST::ServiceRegister<HelloWorldService> HelloWorldService::reg("HelloWorldService");

