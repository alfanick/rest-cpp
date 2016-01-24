#include "uniform.h"

namespace REST {

namespace Dispatchers {

int Uniform::next_worker_id() {
  return distribution(engine);
}

}

}
