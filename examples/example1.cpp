//
// Created by michele on 15.01.23.
//

#include "server.h"
#include "middleware/json_middleware.h"
#include "lib/json/json_entity.h"
#include <iostream>

using namespace std;
using namespace Espresso::JSON;
using namespace Espresso;

int main() {
  Espresso::Server server;

  server.middlewares.use(JSONMiddleware);
  server.middlewares.use([](HTTPRequest &request,
                            HTTPResponse &response,
                            auto next) {
    cout << "Middleware" << endl;
    next();
  });

  server.router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });

  server.router.get("/test/:id",
                    [](HTTPRequest &request, HTTPResponse &response) {
                      response.send("Hello " + request.params["id"]);
                    });

  server.router.post("/test", [](HTTPRequest &request, HTTPResponse &response) {
    const JSONEntity &json = request.getJSON();
    response.send("Hello " + json.toJSON());
  });

  server.router.get("/*", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("404");
  });

  server.listen(8888, []() {
    std::cout << "Server started" << std::endl;
  });
  return 0;
}