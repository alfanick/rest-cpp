#ifndef REST_CPP_SERVICE_H
#define REST_CPP_SERVICE_H

#include <functional>
#include <iostream>
#include <map>
#include <algorithm>
#include "request.h"
#include "response.h"
#include "exceptions.h"

namespace REST {

class Worker;

/**
 * Service provice RESTful stuff to other classes.
 *
 * Classes must inherit REST::Service to be open to public.
 */
class Service {
  friend class Worker;

  public:
    Service();
    //! \private
    virtual ~Service() = 0;

    std::shared_ptr<Response> response;
    Request::shared request;
    std::shared_ptr<Session> session;

    void ensure_session();
    void ensure_authorization(std::string const& realm, std::function<bool(std::string, std::string)> handler);

  protected:

    virtual void make_action();

    virtual void before();
    virtual void after();

    virtual void method(Request::Method method);
};

}

#endif
