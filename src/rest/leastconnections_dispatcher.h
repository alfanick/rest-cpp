#ifndef REST_CPP_DISPATCHER_LEASTCONNECTIONS_H
#define REST_CPP_DISPATCHER_LEASTCONNECTIONS_H

#include "dispatcher.h"

namespace REST {

/**
 * Selects next Worker with least connections in its queue.
 */
class LeastConnectionsDispatcher : public Dispatcher {
  public:
    LeastConnectionsDispatcher(int workers_count) : Dispatcher(workers_count) { };

  protected:
    int next_worker_id();

  private:
    int best_worker_id = 0;
    int best_worker_queue_size = 10000;

    std::mutex best_worker_lock;
};

}

#endif
