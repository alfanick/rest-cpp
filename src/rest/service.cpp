#include "service.h"

namespace REST {

  Service::Service() {}

  void Service::before() {}
  void Service::after() {}

  void Service::method(Request::Method method) {
    throw HTTP::NotImplemented();
  }

  void Service::make_action() {
    before();

    method(request->method);

    after();
  }

  void Service::ensure_session() {
    if (session == nullptr) {
      session = Session::getSession(Session::generateId());
    }
  }

  void Service::ensure_authorization(std::string const& realm, std::function<bool(std::string, std::string)> handler) {
    if (request->authorization.first.empty() || !handler(request->authorization.first, request->authorization.second))
      response->authorization(realm);
  }
}
