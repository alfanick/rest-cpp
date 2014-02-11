#include "response.h"

namespace REST {

Json::FastWriter Response::json_writer;

Response::Response(std::shared_ptr<Request> request) {
  handle = request->handle;
  headers["Content-Type"] = "text/plain";
}

Response::Response(std::shared_ptr<Request> request, HTTP::Error &error) : Response(request) {
  status = error.code();
  status_message = error.what();
  raw = error.what();
}

void Response::use_json() {
  headers["Content-Type"] = "application/json";
  is_json = true;
}

size_t Response::send() {
  size_t bytes_sent = 0;

  std::string payload = is_json ? json_writer.write(data) : raw;

  // start http
  std::string content = "HTTP/1.0 " + std::to_string(status) + " " + status_message + "\r\n";

  // content size
  headers["Content-Length"] = std::to_string(payload.size());

  // add headers
  for (auto header : headers)
    content += header.first + ": " + header.second + "\r\n";

  content += "\r\n";

  content += payload;

  // send every byte
  while ((bytes_sent += ::send(handle, content.c_str(), content.size(), 0)) != content.size());

  // close connection with client
  close(handle);

  return bytes_sent;
}

Response::~Response() {
}

}
