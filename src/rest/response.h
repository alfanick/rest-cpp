#ifndef REST_CPP_RESPONSE_H
#define REST_CPP_RESPONSE_H

#include "exceptions.h"
#include "request.h"
#include "json/json.h"

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

    int status = 200;
    std::string status_message = "OK";
    std::string raw;
    std::map< std::string, std::string > headers;

    void use_json();
    void authorization(std::string realm);

    Json::Value data;

    static Json::FastWriter json_writer;

  private:
    int handle;
    bool is_json = false;
};

}

#endif
