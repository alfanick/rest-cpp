#ifndef REST_CPP_UTILS_H
#define REST_CPP_UTILS_H

#include <ctime>
#include <string>
#include <sstream>

namespace REST {
namespace Utils {

template <typename R, typename S>
R parse_string(const S& str) {
  std::stringstream buf;
  buf << str;
  R val;
  buf >> val;
  return val;
}

std::string random_uuid();
std::string uri_decode(const std::string& src);
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);
std::string rfc1123_datetime(time_t time);

}
}

#endif
