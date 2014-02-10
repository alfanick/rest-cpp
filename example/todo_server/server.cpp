#include <rest/server.h>

int main() {
  std::unique_ptr<REST::Server> server (new REST::Server("0.0.0.0", 8080));

  server->run();

}
