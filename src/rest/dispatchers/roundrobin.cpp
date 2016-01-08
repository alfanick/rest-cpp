#include "roundrobin.h"

namespace REST {

namespace Dispatchers {

int RoundRobin::next_worker_id() {
  if (workers_count == 1)
    return 0;

  std::lock_guard<std::mutex> guard(last_worker_id_lock);

  last_worker_id++;
  last_worker_id %= workers_count;

  return last_worker_id;
}

}

}
