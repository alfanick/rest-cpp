#include "service.h"
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    void read() {
      response->raw = "{ message: \"Hello "+request->parameters["name"]+"!\" }";
    }
    void create() {}
    void update() {}
    void destroy() {}
  private:
    static REST::ServiceRegister<HelloWorldService> reg;
};

REST::ServiceRegister<HelloWorldService> HelloWorldService::reg("HelloWorldService");

