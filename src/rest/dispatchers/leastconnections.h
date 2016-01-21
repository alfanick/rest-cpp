#ifndef REST_CPP_DISPATCHER_LEASTCONNECTIONS_H
#define REST_CPP_DISPATCHER_LEASTCONNECTIONS_H

#include "../dispatcher.h"

namespace REST {

namespace Dispatchers {

/**
 * Selects next Worker with least connections in its queue.
 */
class LeastConnections final : public Dispatcher {
  public:
    LeastConnections(int workers_count, int sc) : Dispatcher(workers_count, sc) { };

  private:
    int next_worker_id();

    int best_worker_id = 0;
    unsigned int best_worker_queue_size = 10000;

    std::mutex best_worker_lock;
};

}

}

#endif
