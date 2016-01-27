#ifndef REST_CPP_DISPATCHER_H
#define REST_CPP_DISPATCHER_H

#include <netinet/in.h>
#include <vector>
#include <queue>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <atomic>

#include "worker.h"
#include "request.h"
#include "router.h"

namespace REST {

/**
 * Dispatcher maintains Worker pool. It distributes
 * requests to specific Workers.
 *
 * @private
 * @see Worker
 * @see Router
 */
class Dispatcher {

  public:
    Dispatcher(int workers_count, int streamers_count);
    virtual ~Dispatcher();

    void next(Request::client const &client);

  protected:
    virtual int next_worker_id() = 0;

    int workers_count = 0;
    int streamers_count = 0;
    std::vector<Worker*> workers;
};

}

#endif
