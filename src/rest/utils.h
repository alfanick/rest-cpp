#ifndef REST_CPP_UTILS_H
#define REST_CPP_UTILS_H

#include <string>

namespace REST {
namespace Utils {

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);

}
}

#endif
