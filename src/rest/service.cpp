#include "service.h"

namespace REST {

  services_map* ServiceFactory::services = NULL;

  std::shared_ptr<Service> ServiceFactory::createInstance(std::string const &name) {
    services_map::iterator iter= Map()->find(name);
    if(iter == Map()->end())
      return NULL;
    return iter->second();
  }

  services_map* ServiceFactory::Map() {
    if(services == NULL)
      services = new services_map;
    return services;
  }

  bool ServiceFactory::exist(std::string const& name) {
    services_map::iterator iter = Map()->find(name);
    if(iter == Map()->end())
      return false;
    return true;
  }


  Service::Service() {}

  void Service::before() {}
  void Service::after() {}
  void Service::read() {
    throw HTTP::MethodNotAllowed();
  }
  void Service::create() {
    throw HTTP::MethodNotAllowed();
  }
  void Service::update() {
    throw HTTP::MethodNotAllowed();
  }
  void Service::destroy() {
    throw HTTP::MethodNotAllowed();
  }

  void Service::make_action() {
    before();
    switch (request->method) {
      case Request::Method::GET:
        read();
        break;
      case Request::Method::POST:
        create();
        break;
      case Request::Method::DELETE:
        destroy();
        break;
      case Request::Method::PUT:
        update();
        break;
      default:
        throw HTTP::MethodNotAllowed();
    }
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
