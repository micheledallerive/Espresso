//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SERVER_H
#define ESPRESSO_SERVER_H

#include <functional>
#include <filesystem>
#include "middleware/Middleware.h"
#include "routing/Router.h"

namespace Espresso {

const int ESPRESSO_MAX_CONNECTIONS = 100;
const unsigned int ESPRESSO_DEFAULT_PORT = 8888;
const std::string ESPRESSO_BASE_PATH = std::filesystem::current_path().string();

using Settings = std::unordered_map<std::string, std::any>;

const Settings ESPRESSO_DEFAULT_SETTINGS = {
    {"BASE_PATH", ESPRESSO_BASE_PATH}
};

extern Settings server_settings;

class Server {
 private:
  unsigned short int port_;
  int socket_;
  int max_connections_;

  void handle_connection_(int client_socket);
 public:
  explicit Server();
  explicit Server(Settings settings);
  ~Server();

  Espresso::MiddlewareList middlewares;
  Espresso::Router router;
  void set_max_connections(int max_connections);
  void
  listen(unsigned short int port = ESPRESSO_DEFAULT_PORT,
         const std::function<void(void)> &callback = nullptr);
};

} // Espresso

#endif //ESPRESSO_SERVER_H
