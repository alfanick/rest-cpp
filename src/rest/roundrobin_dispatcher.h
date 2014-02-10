#ifndef REST_CPP_DISPATCHER_ROUNDROBIN_H
#define REST_CPP_DISPATCHER_ROUNDROBIN_H

#include "dispatcher.h"

namespace REST {

/**
 * Selects next not yet used Worker (Round Robin algorithm).
 */
class RoundRobinDispatcher : public Dispatcher {
  public:
    RoundRobinDispatcher(int workers_count) : Dispatcher(workers_count) {};

  protected:
    int next_worker_id();

  private:
    int last_worker_id = -1;
};

}

#endif
