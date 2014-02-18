#include "resource.h"

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
}

}
