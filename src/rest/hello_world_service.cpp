#include "service.h"
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    void read() {
      std::cout << "Hello world!" << std::endl;
    }
  private:
    static REST::ServiceRegister<HelloWorldService> reg;
};

REST::ServiceRegister<HelloWorldService> HelloWorldService::reg("HelloWorldService");

