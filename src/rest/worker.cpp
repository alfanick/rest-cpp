#include "worker.h"
#include "service.h"
#include "router.h"

#include <iostream>

namespace REST {

int Worker::POOL_SIZE = 256;

Worker::Worker(int i, size_t* rc) :
 id(i), requests_count(rc) {
  THREAD_NAME("rest-cpp - main thread");
  server_header = "rest-cpp, worker " + std::to_string(id);
  run();
}

void Worker::run() {
  should_run = true;

  thread = std::thread([this] () {
    THREAD_NAME(server_header.c_str());

    // while worker is alive
    while (should_run) {
      std::unique_lock<std::mutex> queue_lock(requests_queue_lock);

      // wait for new request
      requests_queue_ready.wait(queue_lock, [this] { return !requests_queue.empty(); });

      // get request
      Request::shared request = requests_queue.front();
      requests_queue.pop();

      Response::shared response(new Response(request));
      response->headers["Server"] = server_header;

      try {
        std::cout << "Request '" << request->path << "' - worker #"<<id<<", handle #"<<request->handle<<"\n";

        make_action(request, response);

        response->send(json_writer);

      } catch (HTTP::Error &e) {
        Response::unique error_response(new Response(request, e));
        error_response->headers.insert(response->headers.begin(), response->headers.end());
        error_response->send(json_writer);
      }

      if ((*requests_count) > 0)
        (*requests_count)--;
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
  thread.join();
}

}
