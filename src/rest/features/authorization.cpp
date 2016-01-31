#include "authorization.h"

namespace REST {
namespace Features {

void Authorization::feature_push() {
  std::string auth_header = request->header("Authorization", "");
  if (!auth_header.empty()) {
    if (auth_header.find("Basic") == 0) {
      std::string decoded = Utils::base64_decode(auth_header.substr(auth_header.find(" ")+1));
      size_t colon = decoded.find(":");
      authorization = std::make_pair(decoded.substr(0, colon), decoded.substr(colon+1));
    }
  }
}

void Authorization::ensure_authorization(std::string const& realm, std::function<bool(std::string, std::string)> handler) {
  if (authorization.first.empty() || !handler(authorization.first, authorization.second)) {
    response->headers["WWW-Authenticate"] = "Basic realm=\"" + realm + "\"";
    throw HTTP::Unauthorized();
  }
}

void Authorization::feature_pop() {
  authorization.first.clear();
  authorization.second.clear();
}

}
}
