#include "worker.h"
#include "service.h"
#include "router.h"

#include <iostream>

namespace REST {

int Worker::POOL_SIZE = 256;

Worker::Worker(int i, std::queue< Request::shared >* rq, std::mutex* re, std::mutex* rl, size_t* rc) :
 id(i), requests_queue(rq), requests_empty(re), requests_lock(rl), requests_count(rc) {
  run();
  server_header = "rest-cpp, worker " + std::to_string(id);
}

void Worker::run() {
  should_run.store(true);

  thread = std::thread([this] () {
    // while worker is alive
    while (should_run.load()) {
      // wait for new requests
      if (requests_queue->empty())
        requests_empty->lock();

      // lock requests queue
      requests_lock->lock();

      // maybe empty even after unlocking, wait some more
      if (requests_queue->empty()) {
        requests_lock->unlock();
        continue;
      }

      // get request
      Request::shared request = requests_queue->front();
      requests_queue->pop();

      // ready
      requests_lock->unlock();

      Response::shared response(new Response(request));
      response->headers["Server"] = server_header;

      try {
        std::cout << "Request '" << request->path << "' - worker #"<<id<<", handle #"<<request->handle<<"\n";

        make_action(request, response);

        response->send();

      } catch (HTTP::Error &e) {
        Response::unique error_response(new Response(request, e));
        error_response->headers.insert(response->headers.begin(), response->headers.end());
        error_response->send();
      }

      if ((*requests_count) > 0)
        (*requests_count)--;
    }

    std::cout << "Stopped worker #" << id << std::endl;
  });
}

void Worker::make_action(Request::shared request, Response::shared response) {
  std::shared_ptr<Service> service = Router::find(request, id);

  if (service == NULL)
    throw HTTP::NotFound();

  service->request = request;
  service->response = response;

  service->make_action();
}

void Worker::stop() {
  should_run.store(false);
  thread.join();
}

}
