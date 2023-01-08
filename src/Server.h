//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SERVER_H
#define ESPRESSO_SERVER_H

#include <functional>
#include "Middleware.h"
#include "Router.h"

const int ESPRESSO_MAX_CONNECTIONS = 100;
const unsigned int ESPRESSO_DEFAULT_PORT = 8888;

namespace Espresso {

class Server {
 private:
  unsigned short int port_;
  int socket_;
  int max_connections_;
  Espresso::MiddlewareList *middlewares_;
  Espresso::Router *router_;

  void handle_connection_(int client_socket);
 public:
  explicit Server();
  ~Server();

  void set_max_connections(int max_connections);
  void
  listen(unsigned short int port = ESPRESSO_DEFAULT_PORT,
         const std::function<void(void)> &callback = nullptr);
  void use(const Espresso::Middleware& middleware);
};

} // Espresso

#endif //ESPRESSO_SERVER_H
