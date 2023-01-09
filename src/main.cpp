#include <iostream>
#include "Server.h"
#include "HTTPMessage.h"

const int PORT = 8888;

int main() {
  auto server = new Espresso::Server({
                                         {"BASE_PATH",
                                             Espresso::ESPRESSO_BASE_PATH
                                                 + "/../../src/"}
                                     });
  server->middlewares->use([](Espresso::HTTPRequest *request,
                              Espresso::HTTPResponse *response,
                              auto next) {
    std::cout << "Hello World!" << std::endl;
    next();
  });
  server->middlewares->use("/test/*", [](Espresso::HTTPRequest *request,
                                         Espresso::HTTPResponse *response,
                                         auto next) {
    std::cout << "Hello World 2!" << std::endl;
  });

  server->router->get("/:test",
                      [](Espresso::HTTPRequest *request,
                         Espresso::HTTPResponse *response) {
                        response->sendFile("./views/index.html");
                      });

  server->listen(PORT, [&]() {
    std::cout << "Listening on port " << PORT << std::endl;
  });
  return 0;
}
