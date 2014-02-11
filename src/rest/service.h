#ifndef REST_CPP_SERVICE_H
#define REST_CPP_SERVICE_H

#include <iostream>
#include <map>
#include "request.h"
#include "response.h"

namespace REST {

/**
 * Service provice RESTful stuff to other classes.
 *
 * Classes must inherit REST::Service to be open to public.
 */
class Service {
  public:
    Service();
    void create();
    void update();
    void destroy();
    void read();
    Response* response;
    Request* request;
    
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
  ServiceRegister(std::string const& name) {
    Map()->insert(std::make_pair(name, &createService<T>));
  }
};

}

#endif
