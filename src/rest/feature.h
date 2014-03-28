#ifndef REST_CPP_FEATURE_H
#define REST_CPP_FEATURE_H

#include "service.h"

namespace REST {

class Service;

class Feature : public virtual Service {
  public:
    Feature();

    virtual std::string feature_name() const = 0;
    virtual void feature_push();
    virtual void feature_pop();
};

}

#endif
