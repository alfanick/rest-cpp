#ifndef REST_CPP_RESPONSE_H
#define REST_CPP_RESPONSE_H

#include "exceptions.h"
#include "request.h"

#include <string>
#include <map>
#include <unistd.h>
#include <iostream>

namespace REST {

/**
 * Response holds response data such as JSON data
 * and status code.
 */
class Response {

  public:
    Response(std::shared_ptr<Request> request);
    Response(std::shared_ptr<Request> request, HTTP::Error &error);
    ~Response();

    size_t send();

    int status;
    std::string status_message;
    std::string raw;
    std::map< std::string, std::string > headers;

    size_t content_length();

    // JSON::Content json;

  private:
    int handle;
};

}

#endif
