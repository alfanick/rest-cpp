#ifndef REST_CPP_REQUEST_H
#define REST_CPP_REQUEST_H

#include <netinet/in.h>
#include <unistd.h>

namespace REST {

/**
 * Basic REST request.
 *
 * Request defines request method, parameters
 * and headers.
 */
class Request {
  public:
    static const size_t BUFFER_SIZE = 16384;

    Request(int client, struct sockaddr_storage client_addr);
    ~Request();

  private:
    char buffer[BUFFER_SIZE];
    size_t length;

    int handle;
    struct sockaddr_storage addr;
};

}

#endif
