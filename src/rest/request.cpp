#include "request.h"
#include <iostream>

namespace REST {

Request::Request(int client, struct sockaddr_storage client_addr) : handle(client), addr(client_addr) {
  length = recv(client, buffer, BUFFER_SIZE, 0);

  std::istringstream request_stream(buffer);
  std::string line;

  bool is_header = true;
  while (std::getline(request_stream, line)) {
    if (method == Method::UNDEFINED) {
      parse_header(line);
      continue;
    }

    line.erase(line.find_last_not_of(" \n\r\t")+1);

    if (line.size() == 0) {
      is_header = false;
      continue;
    }

    if (is_header) {
      size_t colon;
      std::string name = line.substr(0, colon = line.find(":"));
      std::string value = line.substr(colon+1, line.size() - colon);
      value.erase(0, value.find_first_not_of(" \n\r\t"));


      headers.insert(std::make_pair(name, value));
      continue;
    } else {

    }
  }

  for (auto header : headers) {
    std::cout << "Name: '"<< header.first <<"' Value: '"<< header.second <<"'\n";
  }

  char *msg = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nlolo\r\n";
  int len;
  ssize_t bytes_sent;
  len = strlen(msg);
  bytes_sent = send(handle, msg, len, MSG_DONTWAIT);

  std::cout << "koncze "<< handle<<"\n";
  close(handle);
}


void Request::parse_header(std::string line) {
  if (line.find("GET") == 0) {
    method = Method::GET;
    std::cout<<"mam geta\n";
  } else
  if (line.find("HEAD") == 0) {
    method = Method::HEAD;
  } else
  if (line.find("POST") == 0) {
    method = Method::POST;
  } else
  if (line.find("PUT") == 0) {
    method = Method::PUT;
  } else
  if (line.find("DELETE") == 0) {
    method = Method::DELETE;
  } else
  if (line.find("TRACE") == 0) {
    method = Method::TRACE;
  } else
  if (line.find("CONNECT") == 0) {
    method = Method::CONNECT;
  } else
  if (line.find("OPTIONS") == 0) {
    method = Method::OPTIONS;
  }

  size_t path_start = line.find_first_of(" ")+1;
  size_t path_end = line.rfind("HTTP/1.")-1;

  path = line.substr(path_start, path_end - path_start);

  std::cout << "sciezka to '"<< path <<"'\n";
}

Request::~Request() {
}

}
