#include "roundrobin_dispatcher.h"

namespace REST {

int RoundRobinDispatcher::next_worker_id() {
  last_worker_id++;
  last_worker_id %= workers_count;
  return last_worker_id;
}

}
