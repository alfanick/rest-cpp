#include "leastconnections.h"
#include <iostream>
#include <algorithm>

namespace REST {

namespace Dispatchers {

int LeastConnections::next_worker_id() {
  return std::distance(workers.begin(), std::min_element(workers.begin(), workers.end(),
    [](Worker::shared const& a, Worker::shared const& b) {
      return a->clients_count < b->clients_count;
    }
  ));
}

}

}
