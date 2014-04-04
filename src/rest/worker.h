#ifndef REST_CPP_WORKER_H
#define REST_CPP_WORKER_H

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

#include "exceptions.h"
#include "response.h"
#include "request.h"
#include "json/json.h"

#include <pthread.h>
#ifdef __APPLE__
#define THREAD_NAME(x) pthread_setname_np((x))
#else
#define THREAD_NAME(x) pthread_setname_np(pthread_self(), (x))
#endif

namespace REST {

/**
 * Worker is single operating thread. Worker can process only
 * one request at a time.
 *
 * @private
 * @see Dispatcher
 */
class Worker final {

  public:
    Worker(int id, std::queue< Request::shared >* requests_queue, std::mutex* requests_empty, std::mutex* requests_lock, size_t* requests_count);

    void make_action(Request::shared request, Response::shared response);

    void stop();

    static int POOL_SIZE;
  private:
    Json::FastWriter json_writer;
    void run();
    std::string server_header;

    int id;
    std::atomic_bool should_run;

    std::queue< Request::shared > *requests_queue;
    std::mutex* requests_empty;
    std::mutex* requests_lock;
    size_t* requests_count;

    std::thread thread;
};

}

#endif
