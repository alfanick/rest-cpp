#include "leastconnections.h"
#include <iostream>
#include <algorithm>

namespace REST {

namespace Dispatchers {

int LeastConnections::next_worker_id() {
  unsigned int min_count = std::numeric_limits<unsigned int>::max();
  int min_id = 0;

  for (int i = 0; i < Worker::POOL_SIZE; i++) {
    if (workers[i]->clients_count < min_count) {
      min_count = workers[i]->clients_count;
      min_id = i;
    }
  }

  return min_id;
}

}

}
