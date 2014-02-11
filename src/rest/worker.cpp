#include "worker.h"

#include <iostream>

namespace REST {

Worker::Worker(int i, std::queue< std::shared_ptr<Request> > *rq, std::mutex *re, std::mutex *rl) :
 id(i), requests_queue(rq), requests_empty(re), requests_lock(rl) {
  std::cout<<"new worker"<<std::endl;
}

void Worker::stop() {
  std::cout << "stopping worker " << id << std::endl;
}

}
