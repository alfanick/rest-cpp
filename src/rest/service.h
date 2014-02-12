#ifndef REST_CPP_SERVICE_H
#define REST_CPP_SERVICE_H

#include <iostream>
#include <map>
#include <algorithm>
#include "request.h"
#include "response.h"
#include "exceptions.h"

namespace REST {

typedef std::map<std::string, std::string> params_map;

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
    virtual void make_action();
    void setParams(std::shared_ptr<params_map>);
    std::shared_ptr<Response> response;
    std::shared_ptr<Request> request;
    std::shared_ptr<params_map> params;
};

template <typename T>
std::shared_ptr<Service> createService() {
  return std::make_shared<T>();
};

typedef std::map<std::string, std::shared_ptr<Service>(*)()> services_map;
class ServiceFactory {
  public:
    static std::shared_ptr<Service> createInstance(std::string const&);
    static bool exist(std::string const&);
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
