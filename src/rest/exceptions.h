#ifndef REST_CPP_EXCEPTIONS_H
#define REST_CPP_EXCEPTIONS_H
#define CREATE(NAME, PARENT, MESSAGE) class NAME : PARENT { public: virtual const char* what() const throw() { return (MESSAGE); } };

#include <exception>

namespace REST {

  CREATE(Exception, std::exception, "Undefined exception");

  CREATE(ServerError, Exception, "Server exception");
  CREATE(AddressResolvingError, ServerError, "Cannot resolve address");
  CREATE(SocketCreationError, ServerError, "Cannot create socket");
  CREATE(PortInUseError, ServerError, "Port is already in use");
}

#undef CREATE

#endif
