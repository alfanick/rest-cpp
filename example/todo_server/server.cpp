#include <rest/server.h>

#include <iostream>
#include <signal.h>

std::unique_ptr<REST::Server> server (new REST::Server("0.0.0.0", 8080));

void stop_server(int a = 0) {
  server.reset();
}

int main() {
  signal(SIGINT, stop_server);

  server->run();

  return 0;
}
