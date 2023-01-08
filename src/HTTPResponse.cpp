//
// Created by michele on 08.01.23.
//

#include "HTTPResponse.h"

namespace Espresso {

HTTPResponse::HTTPResponse() : HTTPMessage() {
  this->status_ = 200;
}

HTTPResponse::~HTTPResponse() = default;

HTTPResponse *HTTPResponse::setStatus(int status) {
  this->status_ = status;
  return this;
}

} // Espresso