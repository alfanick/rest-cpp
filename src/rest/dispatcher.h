#ifndef REST_CPP_DISPATCHER_H
#define REST_CPP_DISPATCHER_H

#include <netinet/in.h>
#include <vector>
#include <queue>
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

#include "worker.h"
#include "request.h"

namespace REST {

/**
 * Dispatcher maintains Worker pool. It distributes
 * requests to specific Workers.
 *
 * @see Worker
 * @see Router
 */
class Dispatcher {

  public:
    Dispatcher(int workers_count);
    virtual ~Dispatcher();

    void next(int client, struct sockaddr_storage client_addr);

  protected:
    virtual int next_worker_id() = 0;

    int workers_count;
    std::vector< std::shared_ptr<Worker> > workers;
    std::vector< std::queue< std::shared_ptr<Request> > > requests;
    std::mutex *requests_empty;
    std::mutex *requests_lock;
};

}

#endif
