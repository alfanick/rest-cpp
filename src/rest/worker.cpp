#include "worker.h"
#include "service.h"
#include "router.h"

#include <csignal>
#include <iostream>

namespace REST {

int Worker::POOL_SIZE = 256;

Worker::Worker(int i, size_t* cc) :
 id(i), clients_count(cc) {
  THREAD_NAME("rest-cpp - main thread");
  server_header = "rest-cpp, worker " + std::to_string(id);
  run();
}

void Worker::run() {
  should_run = true;

  thread = std::thread([this] () {
    THREAD_NAME(server_header.c_str());

    signal(SIGPIPE, SIG_IGN);

    // while worker is alive
    while (should_run) {
      std::unique_lock<std::mutex> queue_lock(clients_queue_lock);

      // wait for new request
      clients_queue_ready.wait(queue_lock, [this] { return !should_run || !clients_queue.empty(); });
      auto client = clients_queue.front();
      clients_queue.pop();

      if (!should_run)
        break;

      // make request
      Request::shared request = Request::make(client);

      Response::shared response(new Response(request, &streamers));
      response->headers["Server"] = server_header;

      try {
        // std::cout << "Request '" << request->path << "' - worker #"<<id<<", handle #"<<request->handle<<"\n";

        make_action(request, response);

        response->send();

      } catch (HTTP::Error &e) {
        Response::unique error_response(new Response(request, e));
        error_response->headers.insert(response->headers.begin(), response->headers.end());
        error_response->send();
      }

      if ((*clients_count) > 0)
        (*clients_count)--;

      if (streamers.size() > 8) {
        for (auto& s : streamers)
          s.join();
        streamers.clear();
      }
    }

    std::cout << "Stopped worker #" << id << std::endl;
  });
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
  for (auto& s : streamers)
    s.join();
  streamers.clear();
  clients_queue_ready.notify_one();
  thread.join();
}

}
