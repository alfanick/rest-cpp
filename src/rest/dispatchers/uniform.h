#ifndef REST_CPP_DISPATCHER_UNIFORM_H
#define REST_CPP_DISPATCHER_UNIFORM_H

#include <random>
#include "../dispatcher.h"

namespace REST {

namespace Dispatchers {

/**
 * Selects next random Worker, using uniform distribution.
 */
class Uniform final : public Dispatcher {
  public:
    Uniform(int workers_count, int sc) : Dispatcher(workers_count, sc), distribution(0, workers_count-1) {};

  private:
    std::uniform_int_distribution<int> distribution;
    std::linear_congruential_engine<unsigned int, 48271, 397, 0> engine;

    int next_worker_id();
};

}

}

#endif
