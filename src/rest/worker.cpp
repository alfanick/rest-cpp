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

      // maybe empty even after unlocking - stop()
      if (requests_queue->empty())
        break;

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
      std::cout << request->handle << " is my handle\n";
      std::cout << id << ": got request\n";

      char *msg = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nlolo\r\n";
      int len;
      ssize_t bytes_sent;
      len = strlen(msg);
      bytes_sent = send(request->handle, msg, len, MSG_DONTWAIT);

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
