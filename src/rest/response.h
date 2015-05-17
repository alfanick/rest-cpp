#ifndef REST_CPP_RESPONSE_H
#define REST_CPP_RESPONSE_H

#include "exceptions.h"
#include "request.h"
#include "json/json.h"
#include "http.h"

#include <chrono>
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
  friend class Worker;

  public:
    typedef std::shared_ptr<Response> shared;
    typedef std::unique_ptr<Response> unique;
    ~Response();

    std::string raw;
    std::map< std::string, std::string > headers;

    void use_json();

    Json::Value data;
    void set_status(HTTP_STATUS::Code);

  private:
    Response(Request::shared request);
    Response(Request::shared request, HTTP::Error &error);
    size_t send(Json::FastWriter &json_writer);

    std::chrono::high_resolution_clock::time_point start_time;
    HTTP_STATUS::Code status = HTTP_STATUS::Code::OK;
    std::string status_message = HTTP_STATUS::messages.at(status);

    int handle;
    bool is_json = false;
};

}

#endif
