#include "dispatcher.h"

namespace REST {

Dispatcher::Dispatcher(int wc) : workers_count(wc) {
  Router::WORKERS = workers_count;
  requests_empty = new std::mutex[workers_count];
  requests_lock = new std::mutex[workers_count];
  requests_count = new size_t[workers_count];
  workers.resize(workers_count);
  requests.resize(workers_count);

  for (int i = 0; i < workers_count; i++) {
    workers[i] = std::make_shared<Worker>(i, &requests[i], &requests_empty[i], &requests_lock[i], &requests_count[i]);
    requests_empty[i].lock(); // empty by default
  }
}


Dispatcher::~Dispatcher() {
  for (int i = 0; i < workers_count; i++) {
    requests_empty[i].try_lock();
    requests_empty[i].unlock();
    workers[i]->stop();
  }

  delete[] requests_empty;
  delete[] requests_lock;
  delete[] requests_count;
}

void Dispatcher::dispatch(int worker_id, std::shared_ptr<Request> request) {
  requests_lock[worker_id].lock();
  requests[worker_id].push(request);
  requests_count[worker_id]++;
  requests_lock[worker_id].unlock();

  requests_empty[worker_id].try_lock();
  requests_empty[worker_id].unlock();
}

void Dispatcher::next(int client, struct sockaddr_storage client_addr) {
  dispatch(next_worker_id(), std::make_shared<Request>(client, client_addr));
}

}
