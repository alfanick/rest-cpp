#include "request.h"
#include <iostream>
#include <chrono>
#include <thread>
namespace REST {

Request::Request(int client, struct sockaddr_storage client_addr) : handle(client), addr(client_addr) {
  length = recv(client, buffer, BUFFER_SIZE, 0);

  std::cout << "received " << length << std::endl;
  std::cout << buffer;

  char *msg = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nlolo\r\n";
  int len;
  ssize_t bytes_sent;
  len = strlen(msg);
  bytes_sent = send(handle, msg, len, MSG_DONTWAIT);

  std::cout << "koncze "<< handle<<"\n";
  close(handle);
}

Request::~Request() {
}

}
