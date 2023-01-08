//
// Created by michele on 08.01.23.
//

#include "Server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <netinet/in.h>
#include <iostream>
#include <functional>

namespace Espresso {
Server::Server() {
  this->_port = -1;
  this->_socket = -1;
  this->_max_connections = ESPRESSO_MAX_CONNECTIONS;
}

Server::~Server() {
  if (this->_socket != -1) {
    close(this->_socket);
  }
}

void Server::set_max_connections(int max_connections) {
  this->_max_connections = max_connections;
}

void Server::listen(unsigned short int port,
                    const std::function<void(void)> &callback) {
  this->_port = port;
  if (this->_socket != -1) {
    close(this->_socket);
  }

  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket == -1) {
    throw std::runtime_error("Could not create socket");
  }
  int opt = 1;
  if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    throw std::runtime_error("Could not set socket options");
  }
  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(this->_port);
  if (::bind(this->_socket, (struct sockaddr *) &address, sizeof(address))
      < 0) {
    throw std::runtime_error("Could not bind socket");
  }
  if (::listen(this->_socket, this->_max_connections) < 0) {
    throw std::runtime_error("Could not listen on socket");
  }

  if (callback) callback();

  while (true) {
    int client_socket = accept(this->_socket, nullptr, nullptr);
    if (client_socket < 0) {
      throw std::runtime_error("Could not accept connection");
    }
    if (fork() == 0) {
      char buffer[1024] = {0};
      read(client_socket, buffer, 1024);
      std::cout << buffer << std::endl;
      send(client_socket, "HTTP/1.1 200 OK\r\n\r\ntest", 23, 0);
      shutdown(client_socket, SHUT_RDWR);
      close(client_socket);
      exit(0);
    }
  }
}
} // Espresso