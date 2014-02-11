#include "service.h"
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    void read() { }
  private:
    static REST::ServiceRegister<HelloWorldService> reg;
};

REST::ServiceRegister<HelloWorldService> HelloWorldService::reg("HelloWorldService");

