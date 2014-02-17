#include "leastconnections_dispatcher.h"
#include <iostream>
namespace REST {

int LeastConnectionsDispatcher::next_worker_id() {
  best_worker_lock.lock();

  best_worker_queue_size = 10000;
  size_t c;
  for (int i = 0; i < workers_count; i++) {
    c = requests_count[i].load();
    std::cout << "worker #" << i << ": " << c << std::endl;
    if (c < best_worker_queue_size) {
      best_worker_queue_size = c;
      best_worker_id = i;

      if (best_worker_queue_size == 0)
        break;
    }
  }

  best_worker_lock.unlock();

  return best_worker_id;
}

}
