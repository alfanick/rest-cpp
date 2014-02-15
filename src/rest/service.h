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

typedef std::map<std::string, std::string> params_map;

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

    virtual void before();
    virtual void after();
    virtual void create();
    virtual void update();
    virtual void destroy();
    virtual void read();
    virtual void make_action();
    std::shared_ptr<Response> response;
    std::shared_ptr<Request> request;
    std::shared_ptr<params_map> params;

    void ensure_session();
    void ensure_authorization(std::string const& realm, std::function<bool(std::string, std::string)> handler);

  protected:
    std::shared_ptr<Session> session = nullptr;
};

}

#endif
