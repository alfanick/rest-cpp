#include "dispatcher.h"

namespace REST {

Dispatcher::Dispatcher(int wc) : workers_count(wc) {
  requests_empty = new std::mutex[workers_count];
  requests_lock = new std::mutex[workers_count];
  workers.resize(workers_count);
  requests.resize(workers_count);

  for (int i = 0; i < workers_count; i++) {
    workers[i] = std::make_shared<Worker>(&requests[i], &requests_empty[i], &requests_lock[i]);
  }
}


Dispatcher::~Dispatcher() {
  delete[] requests_empty;
  delete[] requests_lock;

  for (int i = 0; i < workers_count; i++) {
    // workers[i].stop();
  }
}

void Dispatcher::next(int client, struct sockaddr_storage client_addr) {
  int worker_id = next_worker_id();



  std::cout << "Klient handler: " << client << std::endl;
  std::cout << "Watek: " << worker_id << std::endl;

  close(client);
}

}
