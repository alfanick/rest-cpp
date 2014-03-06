#ifndef REST_CPP_SERVER_H
#define REST_CPP_SERVER_H

#include <string>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "exceptions.h"
#include "dispatchers/roundrobin.h"
#include "dispatchers/leastconnections.h"
#include "dispatchers/uniform.h"
#include "router.h"

namespace REST {

/**
 * Server creates socket and listens for incoming
 * connections. Connections are maintained by Dispatcher.
 *
 * @see Dispatcher
 */
class Server {

  public:
    Server(std::string path, Dispatcher* d);
    Server(std::string address, int port, Dispatcher* d);
    ~Server();

    void run();
    Router* router();

  private:
    Dispatcher* dispatcher;

    bool is_running = true;

    struct addrinfo host_info;
    struct addrinfo* host_info_list;
    int handle;
};

}

#endif
