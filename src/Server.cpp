//
// Created by michele on 08.01.23.
//

#include "Server.h"
#include "HTTPRequest.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <netinet/in.h>
#include <iostream>
#include <functional>
#include <memory>
#include <utility>

namespace Espresso {

Settings server_settings = ESPRESSO_DEFAULT_SETTINGS;

Server::Server() {
  this->port_ = -1;
  this->socket_ = -1;
  this->max_connections_ = ESPRESSO_MAX_CONNECTIONS;
  this->middlewares = new Espresso::MiddlewareList();
  this->router = new Espresso::Router();
}
Server::Server(Settings settings) : Server() {
  server_settings = std::move(settings);
}

Server::~Server() {
  if (this->socket_ != -1) {
    close(this->socket_);
  }
  delete this->middlewares;
  delete this->router;
}

void Server::set_max_connections(int max_connections) {
  this->max_connections_ = max_connections;
}

void Server::listen(unsigned short int port,
                    const std::function<void(void)> &callback) {
  this->port_ = port;
  if (this->socket_ != -1) {
    close(this->socket_);
  }

  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socket_ == -1) {
    throw std::runtime_error("Could not create socket");
  }
  int opt = 1;
  if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    throw std::runtime_error("Could not set socket options");
  }
  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(this->port_);
  if (::bind(this->socket_, (struct sockaddr *) &address, sizeof(address))
      < 0) {
    throw std::runtime_error("Could not bind socket");
  }
  if (::listen(this->socket_, this->max_connections_) < 0) {
    throw std::runtime_error("Could not listen on socket");
  }

  if (callback) callback();

  while (true) {
    int client_socket = accept(this->socket_, nullptr, nullptr);
    if (client_socket < 0) {
      throw std::runtime_error("Could not accept connection");
    }
    if (fork() == 0) {
      this->handle_connection_(client_socket);
      exit(0);
    }
  }
}

void Server::handle_connection_(int client_socket) {
  char buffer[2048] = {0};
  read(client_socket, buffer, 2048);

  auto request = new Espresso::HTTPRequest(buffer);
  auto response = new Espresso::HTTPResponse();

  this->middlewares->run(request, response);

  this->router->executeMatchingRoute(request, response);

  send(client_socket, response->toString().c_str(),
       response->toString().length(), 0);

  shutdown(client_socket, SHUT_RDWR);
  close(client_socket);
}
} // Espresso