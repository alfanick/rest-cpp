#ifndef REST_CPP_WORKER_H
#define REST_CPP_WORKER_H

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

#include "exceptions.h"
#include "request.h"

namespace REST {

/**
 * Worker is single operating thread. Worker can process only
 * one request at a time.
 *
 * @see Dispatcher
 */
class Worker {

  public:
    Worker(int id, std::queue< std::shared_ptr<Request> > *requests_queue, std::mutex *requests_empty, std::mutex *requests_lock);

    void stop();

  private:
    void run();

  protected:
    int id;
    std::atomic_bool should_run;

    std::queue< std::shared_ptr<Request> > *requests_queue;
    std::mutex *requests_empty;
    std::mutex *requests_lock;

    std::thread thread;
};

}

#endif
