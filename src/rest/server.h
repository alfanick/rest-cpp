#ifndef REST_CPP_SERVER_H
#define REST_CPP_SERVER_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "exceptions.h"
#include "roundrobin_dispatcher.h"
#include "leastconnections_dispatcher.h"
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
    static const int BACKLOG_SIZE = 1000;

    Server(std::string address, int port, int threads);
    ~Server();

    void run();
    Router* router();

  private:
    Dispatcher* dispatcher;

    bool is_running = true;

    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    int handle;
};

}

#endif
