#ifndef REST_CPP_WORKER_H
#define REST_CPP_WORKER_H

#include <queue>
#include <thread>
#include <atomic>

#include "readerwriterqueue/readerwriterqueue.h"

#include "exceptions.h"
#include "response.h"
#include "request.h"

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
    Worker(int id, int sc);

    void make_action(Request &request, Response &response);
    void stop();

    std::atomic_uint clients_count;
    unsigned long long total_clients_count = 0;

    void enqueue(Request::client const &client);

    static int POOL_SIZE;
  private:
    void run();
    inline void clear_streamers(bool force = false);
    std::string server_header;

    int id;
    bool should_run;

    unsigned int streamers_count;

    std::thread thread;
    std::vector<std::thread> streamers;

    moodycamel::BlockingReaderWriterQueue<Request::client> clients_queue;
};

}

#endif
