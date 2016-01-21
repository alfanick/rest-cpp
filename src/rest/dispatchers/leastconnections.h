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
};

}

}

#endif
