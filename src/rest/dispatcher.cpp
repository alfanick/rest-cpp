#include "dispatcher.h"

namespace REST {

Dispatcher::Dispatcher(int wc, int sc) : workers_count(wc), streamers_count(sc) {
  Worker::POOL_SIZE = workers_count;
  clients_count.resize(workers_count);
  workers.resize(workers_count);

  for (int i = 0; i < workers_count; i++) {
    workers[i] = std::make_shared<Worker>(i, sc, &clients_count[i]);
  }
}


Dispatcher::~Dispatcher() {
  for (int i = 0; i < workers_count; i++) {
    workers[i]->stop();
  }
}

void Dispatcher::dispatch(int worker_id, Request::client client) {
  workers[worker_id]->clients_queue.push(client);
  std::unique_lock<std::mutex> lock(workers[worker_id]->clients_queue_lock);

  clients_count[worker_id]++;

  lock.unlock();
  workers[worker_id]->clients_queue_ready.notify_one();

}

void Dispatcher::next(Request::client client) {
  dispatch(next_worker_id(), client);
}

}
