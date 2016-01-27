#include "server.h"
#include <csignal>

namespace REST {

Router* Server::router() {
  return Router::instance();
}

Server::Server(std::string path, Dispatcher* d) : dispatcher(d) {
  srand(time(0));
  signal(SIGPIPE, SIG_IGN);
  Router::instance();

  unlink(path.c_str());
  struct sockaddr_un local;
  handle = socket(AF_UNIX, SOCK_STREAM, 0);
  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, path.c_str());

  if (bind(handle, (struct sockaddr *)&local, strlen(local.sun_path) + sizeof(local.sun_family) + 1) == -1)
    throw PortInUseError();

}

Server::Server(std::string address, int port, Dispatcher* d) : dispatcher(d) {
  srand(time(0));
  signal(SIGPIPE, SIG_IGN);
  Router::instance();
  int status;

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &host_info, &host_info_list);
  if (status != 0)
    throw AddressResolvingError();

  handle = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
  if (handle == -1)
    throw SocketCreationError();

  int yes = 1;
  status = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(handle, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1)
    throw PortInUseError();
}

Server::~Server() {
  is_running = false;

  delete dispatcher;

  freeaddrinfo(host_info_list);
  close(handle);

#ifdef SERVER_PATH
  unlink(STR(SERVER_PATH));
#endif
}

void Server::run() {
  int status;

  router()->print();

  status = listen(handle, SOMAXCONN);
  if (status == -1)
    throw ServerError();

  Request::client client;
  socklen_t addr_size = sizeof(client.address);
  while (is_running) {
    client.handle = accept(handle, (struct sockaddr *)&(client.address), &addr_size);

    try {
      if (client.handle == -1)
        throw ServerError();

      dispatcher->next(client);
    } catch (Exception &e) {
      if (is_running)
        std::cerr << "!!! " << e.what() << std::endl;
      close(client.handle);
    }
  }
}


}
