#include <rest/server.h>

#include <iostream>

int main() {

  std::cout<<"foobar"<<std::endl;
  std::unique_ptr<REST::Server> server (new REST::Server("0.0.0.0", 8080));



  server->run();

}
