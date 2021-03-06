#ifndef REST_CPP_REQUEST_H
#define REST_CPP_REQUEST_H

#include <chrono>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <memory>
#include <sstream>
#include "utils.h"
#include "json.hpp"

namespace REST {

class Worker;
class Response;
/**
 * Basic REST request.
 *
 * Request defines request method, parameters
 * and headers.
 */
class Request {
  friend class Dispatcher;
  friend class Worker;
  friend class Response;

  struct lowercase_comparator {
    bool operator() (const std::string& lhs, const std::string& rhs) const {
      return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
    }
  };

  public:
    typedef struct {
      struct sockaddr_storage address;
      int handle;
    } client;
    enum class Method { GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT, OPTIONS, PATCH, UNDEFINED };

    ~Request();

    Method method = Method::UNDEFINED;
    std::string path;

    std::map< std::string, std::string, lowercase_comparator > headers;
    std::map< std::string, std::string > parameters;

    std::string raw;
    size_t length = 0;

    std::string header(std::string const& key, const char default_value[]) const {
      auto h = headers.find(key);
      if (h == headers.end())
        return default_value;
      else
        return h->second;
    }

    std::string header(std::string const& key, const std::string& default_value) const {
      auto h = headers.find(key);
      if (h == headers.end())
        return default_value;
      else
        return h->second;
    }

    template <class T>
    T header(std::string const& key, const T& default_value) const {
      auto h = headers.find(key);
      if (h == headers.end())
        return default_value;
      else
        return Utils::parse_string<T>(h->second);
    }

    std::string parameter(std::string const& key, const char default_value[]) const {
      auto p = parameters.find(key);
      if (p == parameters.end())
        return default_value;
      else
        return p->second;
    }

    std::string parameter(std::string const& key, const std::string& default_value) const {
      auto p = parameters.find(key);
      if (p == parameters.end())
        return default_value;
      else
        return p->second;
    }

    template <class T>
    T parameter(std::string const& key, const T& default_value) const {
      auto p = parameters.find(key);
      if (p == parameters.end())
        return default_value;
      else
        return Utils::parse_string<T>(p->second);
    }

    nlohmann::json data;

  private:
    Request(int client, struct sockaddr_storage client_addr);
    void process();

    const static size_t BUFFER_SIZE;

    void parse_header(std::string const &line);
    void parse_query_string(std::string &query);
    std::chrono::high_resolution_clock::time_point time;

    int handle;
    struct sockaddr_storage addr;
};

}

#endif
