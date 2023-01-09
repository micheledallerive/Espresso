//
// Created by michele on 08.01.23.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include "HTTPResponse.h"
#include "Server.h"

namespace Espresso {

HTTPResponse::HTTPResponse() : HTTPMessage() {
  this->status_ = 200;
  this->version_ = "HTTP/1.1";
}

HTTPResponse::~HTTPResponse() = default;

HTTPResponse *HTTPResponse::setStatus(int status) {
  this->status_ = status;
  return this;
}

std::string HTTPResponse::toString() {
  std::string response =
      this->getVersion() + " " + std::to_string(this->status_) + " " + "OK"
          + "\n";
  response += this->headersToString();
  response += "\n";
  response += this->getBody();
  return response;
}

void HTTPResponse::sendFile(const std::string &path) {
  const std::string
      final_path =
      path[0] == '/' ? path : (
          any_cast<std::string>(server_settings["BASE_PATH"]) + path);
  std::cout << final_path << std::endl;
  std::ifstream file(final_path);
  if (file.is_open()) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    this->setBody(buffer.str());
    this->setHeader("Content-Length", std::to_string(buffer.str().length()));
    this->setStatus(200);
  } else {
    throw std::runtime_error("File not found");
  }
}

void HTTPResponse::send(const std::string &body) {
  this->setBody(body);
  this->setHeader("Content-Length", std::to_string(body.length()));
  this->setStatus(200);
}

} // Espresso