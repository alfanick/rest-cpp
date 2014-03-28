#ifndef REST_CPP_RESPONSE_H
#define REST_CPP_RESPONSE_H

#include "exceptions.h"
#include "request.h"
#include "json/json.h"

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


    int status = 200;
    std::string status_message = "OK";
    std::string raw;
    std::map< std::string, std::string > headers;

    void use_json();

    Json::Value data;


  private:
    static Json::FastWriter json_writer;

    Response(Request::shared request);
    Response(Request::shared request, HTTP::Error &error);
    size_t send();

    std::chrono::high_resolution_clock::time_point start_time;

    int handle;
    bool is_json = false;
};

}

#endif
