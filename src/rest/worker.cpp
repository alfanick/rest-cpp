#include "worker.h"
#include "service.h"
#include "router.h"

#include <iostream>

namespace REST {

Worker::Worker(int i, std::queue< std::shared_ptr<Request> > *rq, std::mutex *re, std::mutex *rl) :
 id(i), requests_queue(rq), requests_empty(re), requests_lock(rl) {
  run();
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
      if (requests_queue->empty())
        continue;

      // get request
      std::shared_ptr<Request> request = requests_queue->front();
      requests_queue->pop();

      // ready
      requests_lock->unlock();



      try {
        // route
        // exec
        // send response
        // close
        std::cout << "path: " << request-> path << std::endl;
        Service* service = Router::getResource(request->path);

        if(service == NULL)
          throw HTTP::NotFound();

        service->read();

      } catch (HTTP::Error &e) {
        std::unique_ptr<Response> response(new Response(request, e));
        response->send();
      }

      std::cout << request->handle << " is my handle\n";
      std::cout << id << ": got request\n";
    }

    std::cout << "stopped worker" << id << std::endl;
  });
}

void Worker::stop() {
  should_run.store(false);
  thread.join();

  std::cout << "stopping worker " << id << std::endl;
}

}
