#ifndef REST_CPP_SERVICE_H
#define REST_CPP_SERVICE_H

#include <iostream>
#include <map>
#include <algorithm>
#include "request.h"
#include "response.h"
#include "exceptions.h"

namespace REST {

/**
 * Service provice RESTful stuff to other classes.
 *
 * Classes must inherit REST::Service to be open to public.
 */
class Service {
  public:
    Service();
    virtual void before();
    virtual void after();
    virtual void create();
    virtual void update();
    virtual void destroy();
    virtual void read();
    std::shared_ptr<Response> response;
    std::shared_ptr<Request> request;
};

template <typename T>
Service * createService() {
  return new T;
};

typedef std::map<std::string, Service*(*)()> services_map;
class ServiceFactory {
  public:
    static Service * createInstance(std::string const&);
    static services_map* Map();
  private:
    static services_map* services;
};

template <typename T>
struct ServiceRegister : ServiceFactory {
  ServiceRegister(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    Map()->insert(std::make_pair(name, &createService<T>));
  }
};

}

#endif
