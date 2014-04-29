#ifndef REST_CPP_SERVICE_H
#define REST_CPP_SERVICE_H

#include <functional>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include "request.h"
#include "response.h"
#include "exceptions.h"

namespace REST {

class Worker;
class Feature;

/**
 * Service provice RESTful stuff to other classes.
 *
 * Classes must inherit REST::Service to be open to public.
 */
class Service {
  friend class Worker;
  friend class Router;

  public:
    typedef std::unique_ptr<Service> unique;
    typedef std::shared_ptr<Service> shared;

    Service();
    //! \private
    virtual ~Service() = 0;

    Response::shared response;
    Request::shared request;

  protected:
    std::vector< Feature* > features;

    virtual void make_action() final;

    virtual void before();
    virtual void after();

    virtual void method(Request::Method method);
};

}

#endif
