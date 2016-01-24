#include "worker.h"
#include "service.h"
#include "router.h"

#include <csignal>
#include <iostream>

namespace REST {

int Worker::POOL_SIZE = 256;

Worker::Worker(int i, int sc) :
 id(i), streamers_count(sc) {
  THREAD_NAME("rest-cpp - main thread");
  clients_count.store(0);
  server_header = "rest-cpp, worker " + std::to_string(id);
  streamers.reserve(sc);
  run();
}

void Worker::enqueue(Request::client const &client) {
  clients_count++;

  std::unique_lock<std::mutex> lock(clients_queue_lock);

  clients_queue.emplace(client);

  lock.unlock();
  clients_queue_ready.notify_one();
}

Request::client Worker::dequeue() {
  Request::client client;

  std::unique_lock<std::mutex> queue_lock(clients_queue_lock);

  clients_queue_ready.wait(queue_lock, [this] { return !clients_queue.empty() || !should_run; });

  client = clients_queue.front();
  clients_queue.pop();

  return client;
}

void Worker::run() {
  should_run = true;

  thread = std::thread([this] () {
    THREAD_NAME(server_header.c_str());

    signal(SIGPIPE, SIG_IGN);

    // while worker is alive
    while (should_run) {
      // make request
      Request::shared request = Request::make(dequeue());

      if (!should_run)
        break;

      Response::shared response(new Response(request, &streamers));
      response->headers["Server"] = server_header + ", waiting " + std::to_string(clients_count);

      try {
        make_action(request, response);

        response->send();
      } catch (HTTP::Error &e) {
        Response::unique error_response(new Response(request, e));
        error_response->headers.insert(response->headers.begin(), response->headers.end());
        error_response->send();
      }

      clear_streamers();

      clients_count--;
    }

    std::cout << "Stopped worker #" << id << " with " << clients_count << " clients" << std::endl;
  });
}

void Worker::clear_streamers(bool force) {
  if (streamers.size() >= streamers_count || force)  {
    for (auto& s : streamers)
      s.join();
    streamers.clear();
  }
}


void Worker::make_action(Request::shared request, Response::shared response) {
  std::shared_ptr<Service> service = Router::find(request, id);

  if (service == nullptr)
    throw HTTP::NotFound();

  service->request = request;
  service->response = response;

  service->make_action();
}

void Worker::stop() {
  should_run = false;
  clear_streamers();
  clients_queue_ready.notify_one();
  thread.join();
}

}
