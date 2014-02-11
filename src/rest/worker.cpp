#include "worker.h"

#include <iostream>

namespace REST {

Worker::Worker(std::queue< std::shared_ptr<Request> > *rq, std::mutex *re, std::mutex *rl) :
 requests_queue(rq), requests_empty(re), requests_lock(rl) {
  std::cout<<"new worker"<<std::endl;
}

}
