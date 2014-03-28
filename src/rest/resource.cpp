#include "resource.h"
#include "feature.h"

namespace REST {

Resource::~Resource() {

}

void Resource::create() {
  throw HTTP::MethodNotAllowed();
}

void Resource::read() {
  throw HTTP::MethodNotAllowed();
}

void Resource::update() {
  throw HTTP::MethodNotAllowed();
}

void Resource::destroy() {
  throw HTTP::MethodNotAllowed();
}

void Resource::make_action() {
  for (auto feature : features) {
    feature->request = request;
    feature->response = response;
    feature->feature_push();
  }

  before();
  switch (request->method) {
    case Request::Method::POST:
      create();
      break;
    case Request::Method::GET:
      read();
      break;
    case Request::Method::PATCH:
    case Request::Method::PUT:
      update();
      break;
    case Request::Method::DELETE:
      destroy();
      break;
    default:
      method(request->method);
  }
  after();

  for (auto feature = features.rbegin(); feature != features.rend(); ++feature) {
    (*feature)->feature_pop();
  }
}

}
