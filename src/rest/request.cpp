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
      auto ct = headers.find("Content-Type");
      if (ct != headers.end()) {
        if (ct->second == "application/x-www-form-urlencoded")
          parse_query_string(line);
      }
    }
  }

  std::cout << "Headers\n";
  for (auto header : headers) {
    std::cout << "Name: '"<< header.first <<"' Value: '"<< header.second <<"'\n";
  }

  std::cout << "Parameters\n";
  for (auto param : parameters) {
    std::cout << "Name: '"<<param.first<<"' Value: '"<<param.second<<"'\n";
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

  size_t path_query = path.find("?");

  if (path_query != std::string::npos) {
    std::string query = path.substr(path_query+1, path.size() - path_query);

    parse_query_string(query);

    path.erase(path_query, query.size()+1);
  }

  std::cout << "sciezka to '"<< path <<"'\n";
}

void Request::parse_query_string(std::string query) {
  std::istringstream query_stream(query);
  std::string pair;
  while (std::getline(query_stream, pair, '&')) {
    std::string name, value;
    size_t value_position = pair.find("=");

    name = pair.substr(0, value_position);
    if (value_position != std::string::npos)
      value = pair.substr(value_position+1, pair.size() - value_position-1);

    parameters[uri_decode(name)] = uri_decode(value);
  }
}

Request::~Request() {
}

std::string Request::uri_decode(const std::string & src) {
  return src;
}

}
