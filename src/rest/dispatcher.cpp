#include "dispatcher.h"

namespace REST {

Dispatcher::Dispatcher(int wc, int sc) : workers_count(wc), streamers_count(sc) {
  Worker::POOL_SIZE = workers_count;
  workers.resize(workers_count);

  for (int i = 0; i < workers_count; i++) {
    workers[i] = new Worker(i, sc);
  }
}


Dispatcher::~Dispatcher() {
  for (int i = 0; i < workers_count; i++) {
    workers[i]->stop();
    delete workers[i];
  }
}

void Dispatcher::next(Request::client const &client) {
  workers[next_worker_id()]->enqueue(client);
}

}
