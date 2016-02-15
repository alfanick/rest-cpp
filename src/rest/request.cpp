#include "request.h"
#include "exceptions.h"
#include <cstring>

namespace REST {

const size_t Request::BUFFER_SIZE = 4096;

Request::Request(int client, struct sockaddr_storage client_addr) : handle(client), addr(client_addr) {
  time = std::chrono::high_resolution_clock::now();

  // TODO config
  struct timeval tv {
    .tv_sec = 30,
    .tv_usec = 0
  };

  // set timeout for reading
  setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
}

void Request::process() {
  std::string line;
  bool is_header = true;
  char buffer[BUFFER_SIZE];
  ssize_t request_length;

  // receive data from client
  request_length = recv(handle, buffer, BUFFER_SIZE, 0);

  // handle timeout
  if (request_length <= 0)
    throw HTTP::RequestTimeout();

  // parse each line
  std::istringstream request_stream(buffer);

  // first line is HTTP header
  std::getline(request_stream, line);
  parse_header(line);

  // parse headers
  while (std::getline(request_stream, line)) {
    // next lines are headers, strip them
    line.erase(line.find_last_not_of(" \n\r\t")+1);

    // if line is empty, then content should follow
    if (line.size() == 0) {
      is_header = false;
      break;
    }

    // extract name and value from header
    size_t colon;
    std::string name = line.substr(0, colon = line.find(":"));
    std::string value = line.substr(colon+1);
    value.erase(0, value.find_first_not_of(" \n\r\t"));

    headers.emplace(name, value);
  }

  // TODO: handle long headers (longer than buffer)
  // if has content
  if (!is_header) {
    // assume proper content length
    size_t content_length = header("Content-Length", 0);

    // TODO: config
    if (content_length > 1024 * 1024 * 128)
      throw HTTP::PayloadTooLarge();

    if (content_length > 0) {
      raw.reserve(content_length);
      // read whats left in header
      length = std::min(content_length, (size_t)(request_length - request_stream.tellg()));
      raw.append(buffer + request_stream.tellg(), length);

      char* large_buffer = new char[content_length];

      // receive some more
      while (length < content_length) {
        memset(large_buffer, 0, content_length);
        ssize_t buffer_length = recv(handle, large_buffer, content_length, 0);

        if (buffer_length <= 0)
          throw HTTP::RequestTimeout();

        raw.append(large_buffer, buffer_length);
        length += buffer_length;
      }

      delete large_buffer;
    }
  }

  // if has some content
  if (!raw.empty()) {
    // try to parse it
    std::string ct = header("Content-Type", "");

    if (!ct.empty()) {
      if (ct == "application/x-www-form-urlencoded") {
        parse_query_string(raw);
      } else
      if (ct == "application/json" || ct == "text/json") {
        try {
          data = nlohmann::json::parse(raw);
        } catch (...) {
          throw HTTP::BadRequest();
        }
      }
    }
  }
}

void Request::parse_header(std::string const &line) {
  if (line.find("GET") == 0) {
    method = Method::GET;
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
  if (line.find("HEAD") == 0) {
    method = Method::HEAD;
  } else
  if (line.find("PATCH") == 0) {
    method = Method::PATCH;
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

  // TODO config
  if (path.size() >= 2048)
    throw HTTP::URITooLong();

  size_t path_query = path.find("?");

  if (path_query != std::string::npos) {
    std::string query = path.substr(path_query+1, path.size() - path_query);

    parse_query_string(query);

    path.erase(path_query, query.size()+1);
  }
}

void Request::parse_query_string(std::string &query) {
  query.erase(query.find_last_not_of(" \n\r\t")+1);
  std::istringstream query_stream(query);
  std::string pair;
  while (std::getline(query_stream, pair, '&')) {
    std::string name, value;
    size_t value_position = pair.find("=");

    name = pair.substr(0, value_position);
    if (value_position != std::string::npos)
      value = pair.substr(value_position+1, pair.size() - value_position-1);

    parameters[Utils::uri_decode(name)] = Utils::uri_decode(value);
  }
}

Request::~Request() {
}


}
