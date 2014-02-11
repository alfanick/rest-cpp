#include "leastconnections_dispatcher.h"
#include <iostream>
namespace REST {

int LeastConnectionsDispatcher::next_worker_id() {
  best_worker_lock.lock();

  best_worker_queue_size = 10000;
  for (int i = 0; i < workers_count; i++) {
    if (requests[i].size() < best_worker_queue_size) {
      best_worker_queue_size = requests[i].size();
      best_worker_id = i;
    }
  }

  best_worker_lock.unlock();

  return best_worker_id;
}

}
