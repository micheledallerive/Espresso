#include <iostream>
#include "Server.h"
#include "HTTPMessage.h"

const int PORT = 8888;

int main() {
  auto server = new Espresso::Server();
  server->middlewares->use([](Espresso::HTTPRequest *request,
                 Espresso::HTTPResponse *response,
                 auto next) {
    std::cout << "Hello World!" << std::endl;
    next();
  });
  server->middlewares->use([](Espresso::HTTPRequest *request,
                 Espresso::HTTPResponse *response,
                 auto next) {
    std::cout << "Hello World 2!" << std::endl;
  });

  server->router->get("/:test",
                      [](Espresso::HTTPRequest *request,
                         Espresso::HTTPResponse *response) {
                        response->setStatus(200);
                        response->setBody("Hello " + request->params["test"]);
                      });

  server->listen(PORT, []() {
    std::cout << "Listening on port " << PORT << std::endl;
  });
  return 0;
}
