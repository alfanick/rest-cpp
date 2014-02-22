#include "request.h"
#include <cstring>

namespace REST {

size_t Request::BUFFER_SIZE = getpagesize();

Request::Request(int client, struct sockaddr_storage client_addr) : handle(client), addr(client_addr) {
  static Json::Reader json_reader;

  std::string line;
  bool is_header = true;
  char* buffer = new char[BUFFER_SIZE];


  // receive data from client
  recv(client, buffer, BUFFER_SIZE, 0);

  // parse each line
  std::istringstream request_stream(buffer);
  while (std::getline(request_stream, line)) {
    // if method is undefined, assumie its first line
    if (method == Method::UNDEFINED) {
      // so parse header
      parse_header(line);
      continue;
    }

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

    headers.insert(std::make_pair(name, value));
  }

  // if has content
  if (!is_header) {
    // assume proper content length
    size_t content_length = header("Content-Length", 0);
    raw.reserve(content_length);

    if (content_length > 0) {
      // read whats left in header
      length = std::min(content_length, (size_t)(BUFFER_SIZE - request_stream.tellg()));
      raw = std::string(buffer, BUFFER_SIZE).substr(request_stream.tellg(), length);

      // receive some more
      while (length < content_length) {
        memset(buffer, 0, BUFFER_SIZE);
        size_t buffer_length = recv(client, buffer, BUFFER_SIZE, 0);
        raw += std::string(buffer, buffer_length);
        length += buffer_length;
      }
    }
    //std::cout << "content o "<<content_length<<" == "<<length<<" == " <<raw.size()<<"\n";
  }

  delete buffer;

  time = std::chrono::high_resolution_clock::now();

  auto auth_header = headers.find("Authorization");
  if (auth_header != headers.end()) {
    if (auth_header->second.find("Basic") == 0) {
      std::string decoded = Utils::base64_decode(auth_header->second.substr(auth_header->second.find(" ")+1));
      size_t colon = decoded.find(":");
      authorization = std::make_pair(decoded.substr(0, colon), decoded.substr(colon+1));

      //std::cout <<"username: '"<<authorization.first<<"' password: '"<<authorization.second<<"'\n";
      //auth_header = Utils::base64_decode(auth_header);
    }
  }

  // if has some content
  if (!raw.empty()) {
    // try to parse it
    auto ct = headers.find("Content-Type");
    if (ct != headers.end()) {
      if (ct->second == "application/x-www-form-urlencoded") {
        parse_query_string(raw);
      } else
      if (ct->second == "application/json" || ct->second == "text/json") {
        data = std::make_shared<Json::Value>();
        json_reader.parse(raw, *data.get(), false);
      }
    }
  }
}

void Request::parse_header(std::string line) {
  if (line.find("GET") == 0) {
    method = Method::GET;
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
  if (line.find("PATCH") == 0) {
    method = Method::PATCH;
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
}

void Request::parse_query_string(std::string query) {
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
