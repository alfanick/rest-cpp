#ifndef REST_CPP_WORKER_H
#define REST_CPP_WORKER_H

#include <queue>
#include <mutex>
#include <condition_variable>
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
  friend class Dispatcher;
  public:
    typedef std::shared_ptr<Worker> shared;

    Worker(int id, int sc);

    void make_action(Request::shared request, Response::shared response);
    void stop();

    static int POOL_SIZE;
    std::atomic_uint clients_count;

  private:
    void run();
    std::string server_header;

    int id;
    bool should_run;

    unsigned int streamers_count;

    std::thread thread;
    std::vector<std::thread> streamers;

    std::queue<Request::client> clients_queue;
    std::mutex clients_queue_lock;
    std::condition_variable clients_queue_ready;
};

}

#endif
