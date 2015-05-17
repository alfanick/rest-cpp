#ifndef REST_CPP_EXCEPTIONS_H
#define REST_CPP_EXCEPTIONS_H
#define CREATE(NAME, PARENT, MESSAGE) class NAME : public PARENT { public: virtual const char* what() const throw() { return (MESSAGE); } virtual int code() {return 401; }};
#define ERROR(NAME, CODE) class NAME : public Error { public: virtual const char* what() const throw() { return STATUS::messages.at((CODE)).c_str(); } virtual int code() { return (int)(CODE); } };

#include <exception>
#include "http.h"

namespace REST {

  /**
   * @private
   */
  CREATE(Exception, std::exception, "Undefined exception");

  CREATE(ServerError, Exception, "Server exception");
  CREATE(AddressResolvingError, ServerError, "Cannot resolve address");
  CREATE(SocketCreationError, ServerError, "Cannot create socket");
  CREATE(PortInUseError, ServerError, "Port is already in use");

  namespace HTTP {
    CREATE(Error, Exception, "Unknown HTTP protocol error");

    ERROR(NotAuthorized, STATUS::Code::Unauthorized);
    ERROR(NotFound, STATUS::Code::NotFound);
    ERROR(MethodNotAllowed, STATUS::Code::MethodNotAllowed);
    ERROR(NotImplemented, STATUS::Code::NotImplemented);
  }
}

#undef CREATE
#undef ERROR

#endif
