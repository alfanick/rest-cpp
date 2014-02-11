#include "worker.h"

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


      // route
      // exec
      // send response
      // close
      //

      try {
        throw HTTP::NotFound();
      } catch (HTTP::Error &e) {
        // new Response(request, e)->send()
        std::string msg = "HTTP/1.0 " + std::to_string(e.code()) + " " + e.what() + "\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(strlen(e.what())+2) + "\r\n\r\n" + e.what() + "\r\n";
        int len = msg.size();
        ssize_t bytes_sent;
        bytes_sent = send(request->handle, msg.c_str(), len, 0);
      }

      std::cout << request->handle << " is my handle\n";
      std::cout << id << ": got request\n";

      close(request->handle);
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
