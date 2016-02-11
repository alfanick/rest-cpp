#ifndef REST_CPP_EXCEPTIONS_H
#define REST_CPP_EXCEPTIONS_H
#define CREATE(NAME, PARENT, MESSAGE) class NAME : public PARENT { public: virtual const char* what() const throw() { return (MESSAGE); } virtual int code() {return 401; }};
#define ERROR(NAME, CODE, MESSAGE) class NAME : public Error { public: virtual const char* what() const throw() { return (MESSAGE); } virtual int code() { return (CODE); } };

#include <exception>

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

    ERROR(BadRequest, 400, "Bad Request");
    ERROR(Unauthorized, 401, "Unauthorized");
    ERROR(NotFound, 404, "Not Found");
    ERROR(MethodNotAllowed, 405, "Method Not Allowed");
    ERROR(RequestTimeout, 408, "Request Timeout");
    ERROR(PayloadTooLarge, 413, "Payload Too Large");
    ERROR(URITooLong, 414, "URI Too Long");
    ERROR(InternalServerError, 500, "Internal Server Error");
    ERROR(NotImplemented, 501, "Not Implemented");
  }
}

#undef CREATE
#undef ERROR

#endif
