#ifndef REST_CPP_RESOURCE_H
#define REST_CPP_RESOURCE_H

#include "service.h"

namespace REST {

class Resource : public Service {
  public:
    virtual void create();
    virtual void read();
    virtual void update();
    virtual void destroy();

    virtual ~Resource() = 0;

  protected:
    void make_action() final;
};

}

#endif
