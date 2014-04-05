#ifndef REST_CPP_RESOURCE_H
#define REST_CPP_RESOURCE_H

#include "service.h"

namespace REST {

class Resource : public virtual Service {
  public:
    virtual void create();
    virtual void read();
    virtual void update();
    virtual void destroy();

    //! \private
    virtual ~Resource() = 0;

  private:
    void method(Request::Method method) final;
};

}

#endif
