#include "uniform.h"

namespace REST {

namespace Dispatchers {

int Uniform::next_worker_id() {
  static std::uniform_int_distribution<int> distribution(0, workers_count-1);
  static std::random_device device;
  static std::mt19937 engine(device());
  static auto next_uniform = std::bind(distribution, engine);

  return next_uniform();
}

}

}
