#include "request.h"
#include <iostream>
namespace REST {

Request::Request(int client, struct sockaddr_storage client_addr) : handle(client), addr(client_addr) {
  length = recv(client, buffer, BUFFER_SIZE, 0);

  std::cout << "received " << length << std::endl;
  std::cout << buffer;
}

Request::~Request() {
  std::cout << "koczne\n";
  close(handle);
}

}
