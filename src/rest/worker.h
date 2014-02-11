#ifndef REST_CPP_WORKER_H
#define REST_CPP_WOKRER_H

#include <queue>
#include <mutex>

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
    Worker(std::queue< std::shared_ptr<Request> > *requests_queue, std::mutex *requests_empty, std::mutex *requests_lock);

    void stop();

  protected:
    std::queue< std::shared_ptr<Request> > *requests_queue;
    std::mutex *requests_empty;
    std::mutex *requests_lock;
};

}

#endif
