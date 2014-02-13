#ifndef REST_CPP_REQUEST_H
#define REST_CPP_REQUEST_H

#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <memory>
#include <map>
#include <sstream>
#include "json/json.h"
#include "session.h"

namespace REST {

template <typename RETURN_TYPE, typename STRING_TYPE>
RETURN_TYPE parse_string(const STRING_TYPE& str) {
  std::stringstream buf;
  buf << str;
  RETURN_TYPE val;
  buf >> val;
  return val;
}

class Worker;
class Response;
/**
 * Basic REST request.
 *
 * Request defines request method, parameters
 * and headers.
 */
class Request {
  friend class Worker;
  friend class Response;

  public:
    enum class Method { GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT, OPTIONS, UNDEFINED };

    static const size_t BUFFER_SIZE = 16384;

    Request(int client, struct sockaddr_storage client_addr);
    ~Request();

    Method method = Method::UNDEFINED;
    std::string path;
    std::multimap< std::string, std::string > headers;
    std::map< std::string, std::string > parameters;

    template <class T>
    const T parameter(std::string const& key, const T& default_value) {
      if (parameters[key].empty())
        return default_value;
      else
        return parse_string<T>(parameters[key]);
    }

    std::shared_ptr<Json::Value> data = nullptr;
    std::shared_ptr<Session> session = nullptr;

    static std::string uri_decode(const std::string& src);

  private:
    void parse_header(std::string line);
    void parse_query_string(std::string query);

    char buffer[BUFFER_SIZE];
    size_t length;

    int handle;
    struct sockaddr_storage addr;
};

}

#endif
