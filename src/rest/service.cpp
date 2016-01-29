#include "service.h"
#include "feature.h"

namespace REST {

  Service::Service() {}
  Service::~Service() {}

  void Service::before() {}
  void Service::after() {}

  void Service::method(Request::Method method) {
    throw HTTP::NotImplemented();
  }

  void Service::make_action() {
    for (auto feature = features.cbegin(); feature != features.cend(); ++feature) {
      (*feature)->feature_push();
    }

    before();

    method(request->method);

    after();

    for (auto feature = features.crbegin(); feature != features.crend(); ++feature) {
      (*feature)->feature_pop();
    }
  }
}
