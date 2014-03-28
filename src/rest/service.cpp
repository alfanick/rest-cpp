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
    for (auto feature : features) {
      feature->request = request;
      feature->response = response;
      feature->feature_push();
    }

    before();

    method(request->method);

    after();

    for (auto feature = features.rbegin(); feature != features.rend(); ++feature) {
      (*feature)->feature_pop();
    }
  }

  void Service::ensure_session() {
    if (session == nullptr) {
      session = Session::getSession(Utils::random_uuid());
    }
  }
}
