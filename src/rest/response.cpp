#include "response.h"

namespace REST {

Response::Response(std::shared_ptr<Request> request) {
  handle = request->handle;
  headers["Content-Type"] = "text/plain";
  status = 200;
  status_message = "OK";
}

Response::Response(std::shared_ptr<Request> request, HTTP::Error &error) : Response(request) {
  status = error.code();
  status_message = error.what();
  raw = error.what();
}

size_t Response::send() {
  size_t bytes_sent = 0;

  headers["Content-Length"] = std::to_string(content_length());

  // start http
  std::string content = "HTTP/1.0 " + std::to_string(status) + " " + status_message + "\r\n";

  // add headers
  for (auto header : headers)
    content += header.first + ": " + header.second + "\r\n";

  content += "\r\n";

  // TODO: implement json
  content += raw;

  // send every byte
  while ((bytes_sent += ::send(handle, content.c_str(), content.size(), 0)) != content.size());

  // close connection with client
  close(handle);

  return bytes_sent;
}

size_t Response::content_length() {
  // TODO: implement json
  return status_message.size();
}

Response::~Response() {
}

}
