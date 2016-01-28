#include "worker.h"
#include "service.h"
#include "router.h"

#include <iostream>

namespace REST {

int Worker::POOL_SIZE = 256;

Worker::Worker(int i, int sc) :
 id(i), streamers_count(sc), clients_queue(1024) {
  THREAD_NAME("rest-cpp - main thread");
  clients_count.store(0);
  server_header = "rest-cpp, worker " + std::to_string(id);
  streamers.reserve(sc);
  run();
}

void Worker::enqueue(Request::client const& client) {
  clients_queue.enqueue(std::move(client));
  clients_count++;
}

void Worker::run() {
  should_run = true;

  thread = std::thread([this] () {
    THREAD_NAME(server_header.c_str());

    // while worker is alive
    while (should_run) {
      Request::client client;

      clients_queue.wait_dequeue(client);

      // make request
      Request request(client.handle, client.address);

      if (!should_run)
        break;

      Response response(request, streamers);
      response.headers["Server"] = server_header + ", waiting " + std::to_string(clients_count);

      try {
        request.process();

        make_action(request, response);
      } catch (HTTP::Error &e) {
        response.error(e);
      }

      response.send();

      clear_streamers();

      clients_count--;
      total_clients_count++;
    }

    std::cout << "Stopped worker #" << id << " with " << clients_count << " clients in queue (processed " << total_clients_count << ")" << std::endl;
  });
}

void Worker::clear_streamers(bool force) {
  if (streamers.size() >= streamers_count || force)  {
    for (auto& s : streamers)
      s.join();
    streamers.clear();
  }
}


void Worker::make_action(Request &request, Response &response) {
  Service* service = Router::find(request, id);

  if (service == nullptr)
    throw HTTP::NotFound();

  service->request = &request;
  service->response = &response;

  service->make_action();
}

void Worker::stop() {
  should_run = false;
  clear_streamers();
  Request::client c;
  clients_queue.enqueue(c);
  thread.join();
}

}
