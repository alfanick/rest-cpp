#include "response.h"

namespace REST {

Json::FastWriter Response::json_writer;

Response::Response(std::shared_ptr<Request> request) {
  handle = request->handle;
  start_time = request->time;
  headers["Content-Type"] = "text/plain; charset=utf-8";
  headers["Connection"] = "close";
}

Response::Response(std::shared_ptr<Request> request, HTTP::Error &error) : Response(request) {
  status = error.code();
  status_message = error.what();
  raw = error.what();
}

void Response::use_json() {
  headers["Content-Type"] = "application/json; charset=utf-8";
  is_json = true;
}

void Response::authorization(std::string realm) {
  headers["WWW-Authenticate"] = "Basic realm=\""+realm+"\"";
  throw HTTP::NotAuthorized();
}

size_t Response::send() {
  size_t bytes_sent = 0;

  std::string payload = is_json ? json_writer.write(data) : raw;

  // start http
  std::string content = "HTTP/1.0 " + std::to_string(status) + " " + status_message + "\r\n";

  // content size
  headers["Content-Length"] = std::to_string(payload.size());
  headers["Server"] += ", took " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000.0f) + "ms";

  // add headers
  for (auto header : headers)
    content += header.first + ": " + header.second + "\r\n";

  content += "\r\n";

  content += payload;

  // send every byte
  bytes_sent = ::send(handle, content.c_str(), content.size(), MSG_DONTWAIT);

  // close connection with client
  close(handle);

  return bytes_sent;
}

Response::~Response() {
}

}
