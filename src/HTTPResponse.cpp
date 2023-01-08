//
// Created by michele on 08.01.23.
//

#include "HTTPResponse.h"

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
      this->version_ + " " + std::to_string(this->status_) + " " + "OK" + "\n";
  response += this->headersToString();
  response += "\n";
  response += this->body_;
  return response;
}

} // Espresso