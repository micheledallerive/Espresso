#include <iostream>
#include "Server.h"
#include "HTTPMessage.h"

int main() {
  auto server = new Espresso::Server();
  server->listen(8080, []() {
    std::cout << "Listening on port 8080" << std::endl;
  });
  return 0;
}
