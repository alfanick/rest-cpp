#ifndef REST_CPP_RESPONSE_H
#define REST_CPP_RESPONSE_H

#ifdef __APPLE__
#define MSG_NOSIGNAL 0
#endif

#include "exceptions.h"
#include "request.h"
#include "json.hpp"

#include <chrono>
#include <thread>
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
    ~Response();


    int status = 200;
    std::string status_message = "OK";
    std::string raw;
    std::map< std::string, std::string > headers;

    void cache(unsigned long t);
    void stream(std::function<void(int)> streamer, bool async=false);
    void stream_async(std::function<void(int)> streamer);

    nlohmann::json data;


  private:
    Response(Request const &request, std::vector<std::thread> &streamers);

    void error(HTTP::Error &e);
    size_t send();

    std::chrono::high_resolution_clock::time_point start_time;

    std::vector<std::thread> &streamers;
    int handle;
    bool is_streamed = false;
};

}

#endif
