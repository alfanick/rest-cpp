#include "response.h"
#include <thread>
#include <future>
#include <csignal>

namespace REST {

Response::Response(Request const &request, std::vector<std::thread> &s) :
  start_time(request.time),
  streamers(s),
  handle(request.handle) {
  headers["Content-Type"] = "text/plain; charset=utf-8";
  headers["Connection"] = "close";
}

void Response::error(HTTP::Error &error) {
  status = error.code();
  status_message = error.what();
  raw.clear();
  data = {
    { "status", "error" },
    { "error", {
      { "code", status },
      { "message", status_message }
    }}
  };
}

void Response::stream_async(std::function<void(int)> streamer) {
  stream(streamer, true);
}

void Response::stream(std::function<void(int)> streamer, bool async) {
  is_streamed = true;

  std::string content = "HTTP/1.1 " + std::to_string(status) + " " + status_message + "\r\n";
  headers.emplace("Date", Utils::rfc1123_datetime(time(0)));

  // add headers
  for (auto header : headers)
    content += header.first + ": " + header.second + "\r\n";

  content += "\r\n";

  // send every byte
  ::send(handle, content.c_str(), content.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

  if (async && streamers.capacity() > 0) {
    int h = handle;
    streamers.emplace_back([streamer, h]() {
      signal(SIGPIPE, SIG_IGN);
      try {
        // std::this_thread::yield();
        streamer(h);
        close(h);
      } catch (...) {
      }
    });
  } else {
    std::this_thread::yield();
    streamer(handle);
    close(handle);
  }
}

size_t Response::send() {
  if (is_streamed)
    return 0;

  size_t bytes_sent = 0;

  std::string payload;

  if (!data.empty()) {
    headers["Content-Type"] = "application/json; charset=utf-8";
    payload = data.dump();
  } else {
    payload = raw;
  }

  // start http
  std::string content = "HTTP/1.1 " + std::to_string(status) + " " + status_message + "\r\n";

  // content size
  headers["Server"] += ", took " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000.0f) + "ms";
  headers.emplace("Content-Length", std::to_string(payload.size()));
  headers.emplace("Date", Utils::rfc1123_datetime(time(0)));

  // add headers
  for (auto header : headers)
    content += header.first + ": " + header.second + "\r\n";

  content += "\r\n";

  content += payload;

  // send every byte
  bytes_sent = ::send(handle, content.c_str(), content.size(), MSG_DONTWAIT | MSG_NOSIGNAL);

  // close connection with client
  close(handle);

  return bytes_sent;
}

void Response::cache(unsigned long t) {
  if (t > 0) {
    headers["Cache-Control"] = "max-age=" + std::to_string(t) + ", public";
    headers["Expires"] = REST::Utils::rfc1123_datetime(time(0) + t);
  } else {
    headers["Cache-Control"] = "no-cache, no-store, must-revalidate";
  }
}

Response::~Response() {
}

}
