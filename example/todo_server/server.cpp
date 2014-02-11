#include <rest/server.h>

#include <iostream>
#include <signal.h>
#include <cstdlib>

REST::Server *server;

void stop_server(int a = 0) {
  delete server;
}

int main(int argc, char **argv) {
  signal(SIGINT, stop_server);

  server = new REST::Server("0.0.0.0", 8080, (argc > 1) ? atoi(argv[1]) : 4);

  server->run();

  return 0;
}
