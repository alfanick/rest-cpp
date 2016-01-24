#ifndef REST_CPP_DISPATCHER_ROUNDROBIN_H
#define REST_CPP_DISPATCHER_ROUNDROBIN_H

#include <mutex>
#include "../dispatcher.h"

namespace REST {

namespace Dispatchers {

/**
 * Selects next not yet used Worker (Round Robin algorithm).
 */
class RoundRobin final : public Dispatcher {
  public:
    RoundRobin(int workers_count, int sc) : Dispatcher(workers_count, sc), last_worker_id(0) {};

  private:
    int next_worker_id();

    std::atomic_uint last_worker_id;
};

}

}

#endif
