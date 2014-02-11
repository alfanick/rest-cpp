#include "roundrobin_dispatcher.h"

namespace REST {

int RoundRobinDispatcher::next_worker_id() {
  last_worker_id_lock.lock();
  last_worker_id++;
  last_worker_id %= workers_count;
  last_worker_id_lock.unlock();
  return last_worker_id;
}

}
