#ifndef REST_REST_H
#define REST_REST_H

#define STR_QUOTE(name) #name
#define STR(macro) STR_QUOTE(macro)

#ifndef SERVER_BIND
#define SERVER_BIND 0.0.0.0
#endif

#ifndef SERVER_PORT
#define SERVER_PORT 8080
#endif

#ifndef SERVER_WORKERS
#define SERVER_WORKERS 4
#endif

#ifdef SERVER_DISPATCHER_lc
#define SERVER_DISPATCHER Dispatchers::LeastConnections
#endif

#ifdef SERVER_DISPATCHER_rr
#define SERVER_DISPATCHER Dispatchers::RoundRobin
#endif

#ifdef SERVER_DISPATCHER_uf
#define SERVER_DISPATCHER Dispatchers::Uniform
#endif

#ifndef SERVER_DISPATCHER
#define SERVER_DISPATCHER Dispatchers::LeastConnections
#endif

#define create_service(NAME) void NAME(REST::Service* service)
#define create_json_service(NAME) inline void NAME##_wrapped(REST::Service*); void NAME(REST::Service* service) { service->response->use_json(); NAME##_wrapped(service); } inline void NAME##_wrapped(REST::Service* service)

#include <iostream>
#include <signal.h>

#include "exceptions.h"
#include "server.h"

/// \file

using namespace REST::HTTP;

void routes(REST::Router*);

namespace REST {

/**
 * @private
 */
REST::Server* server_instance;

//! \private
void main_stop_server(int a = 0) {
  delete server_instance;
}

int main(int argc, char **argv) {
  signal(SIGINT, main_stop_server);

#ifndef SERVER_PATH
  std::cout << "Listening on " << STR(SERVER_BIND) << ":" << SERVER_PORT << ", " << SERVER_WORKERS << " workers, " << STR(SERVER_DISPATCHER) << "\n";
  server_instance = new REST::Server(STR(SERVER_BIND), SERVER_PORT, new REST::SERVER_DISPATCHER(SERVER_WORKERS));
#else
  std::cout << "Listening on Unix socket " << STR(SERVER_PATH) << ", " << SERVER_WORKERS << " workers, " << STR(SERVER_DISPATCHER) << "\n";
  server_instance = new REST::Server(STR(SERVER_PATH), new REST::SERVER_DISPATCHER(SERVER_WORKERS));
#endif

  ::routes(server_instance->router());

  server_instance->run();

  return 0;
}

}

#ifndef WITHOUT_MAIN
int main(int argc, char **argv) {
  return REST::main(argc, argv);
}
#endif

#endif
