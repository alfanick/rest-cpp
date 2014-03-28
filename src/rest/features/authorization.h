#ifndef REST_CPP_FEATURES_AUTHORIZATION_H
#define REST_CPP_FEATURES_AUTHORIZATION_H

#include "../feature.h"
#include <functional>

namespace REST {
namespace Features {

class Authorization : public Feature {
  public:
    virtual std::string feature_name() const { return "authorization"; }

    std::pair< std::string, std::string > authorization;
    void ensure_authorization(std::string const& realm, std::function<bool(std::string, std::string)> handler);

    void feature_push();
    void feature_pop();
};

}
}

#endif
