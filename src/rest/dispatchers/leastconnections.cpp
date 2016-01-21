#include "leastconnections.h"
#include <iostream>
#include <algorithm>

namespace REST {

namespace Dispatchers {

int LeastConnections::next_worker_id() {
  return std::distance(clients_count.begin(), std::min_element(clients_count.begin(), clients_count.end()));
}

}

}
