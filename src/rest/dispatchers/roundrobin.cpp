#include "roundrobin.h"

namespace REST {

namespace Dispatchers {

int RoundRobin::next_worker_id() {
  return last_worker_id++ % workers_count;
}

}

}
