//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SERVER_H
#define ESPRESSO_SERVER_H

#include <functional>

const int ESPRESSO_MAX_CONNECTIONS = 100;
const unsigned int ESPRESSO_DEFAULT_PORT = 8888;

namespace Espresso {

class Server {
 private:
  unsigned short int _port;
  int _socket;
  int _max_connections;
 public:
  explicit Server();

  ~Server();

  void set_max_connections(int max_connections);

  void
  listen(unsigned short int port = ESPRESSO_DEFAULT_PORT,
         const std::function<void(void)> &callback = nullptr);
};

} // Espresso

#endif //ESPRESSO_SERVER_H
