#include "leastconnections_dispatcher.h"
#include <iostream>
namespace REST {

namespace Dispatchers {

int LeastConnections::next_worker_id() {
  best_worker_lock.lock();

  best_worker_queue_size = 10000;
  for (int i = 0; i < workers_count; i++) {
    std::cout << "worker #" << i << ": " << requests_count[i] << std::endl;
    if (requests_count[i] < best_worker_queue_size) {
      best_worker_queue_size = requests_count[i];
      best_worker_id = i;

      if (best_worker_queue_size == 0)
        break;
    }
  }

  best_worker_lock.unlock();

  return best_worker_id;
}

}

}
