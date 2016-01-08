#include "leastconnections.h"
#include <iostream>
namespace REST {

namespace Dispatchers {

int LeastConnections::next_worker_id() {
  std::lock_guard<std::mutex> guard(best_worker_lock);

  // std::cout << "Workers: ";

  best_worker_queue_size = 10000;
  for (int i = 0; i < workers_count; i++) {
    // std::cout << i << ":" << requests_count[i] << ' ';
    if (requests_count[i] < best_worker_queue_size) {
      best_worker_queue_size = requests_count[i];
      best_worker_id = i;

      if (best_worker_queue_size == 0)
        break;
    }
  }

  // std::cout << std::endl;

  return best_worker_id;
}

}

}
