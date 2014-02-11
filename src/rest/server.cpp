#include "server.h"

namespace REST {


Server::Server(std::string address, int port) {
  dispatcher = new RoundRobinDispatcher(4);//std::thread::hardware_concurrency());
  int status;

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &host_info, &host_info_list);
  if (status != 0)
    throw new AddressResolvingError();

  handle = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
  if (handle == -1)
    throw new SocketCreationError();

  int yes = 1;
  status = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(handle, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1)
    throw new PortInUseError();
}

Server::~Server() {
  delete dispatcher;

  freeaddrinfo(host_info_list);
  close(handle);
}

void Server::run() {
  int status;

  status = listen(handle, BACKLOG_SIZE);
  if (status == -1)
    throw new ServerError();

  while (is_running) {
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int client = accept(handle, (struct sockaddr *)&client_addr, &addr_size);

    try {
      if (client == -1)
        throw new ServerError();

      dispatcher->next(client, client_addr);
    } catch (Exception e) {
      std::cerr << "!!! " << e.what() << std::endl;
      close(client);
    }
  }
}


}
