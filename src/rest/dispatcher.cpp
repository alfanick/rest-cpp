#include "dispatcher.h"

namespace REST {

Dispatcher::Dispatcher(int wc) : workers_count(wc) {
  Worker::POOL_SIZE = workers_count;
  requests_count = new size_t[workers_count];
  workers.resize(workers_count);

  for (int i = 0; i < workers_count; i++) {
    workers[i] = std::make_shared<Worker>(i, &requests_count[i]);
  }
}


Dispatcher::~Dispatcher() {
  for (int i = 0; i < workers_count; i++) {
    workers[i]->stop();
  }
}

void Dispatcher::dispatch(int worker_id, Request::client client) {
  std::unique_lock<std::mutex> lock(workers[worker_id]->requests_queue_lock);

  workers[worker_id]->requests_queue.push(Request::make(client));
  requests_count[worker_id]++;

  workers[worker_id]->requests_queue_ready.notify_one();

}

void Dispatcher::next(Request::client client) {
  dispatch(next_worker_id(), client);
}

}
