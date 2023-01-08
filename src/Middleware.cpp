//
// Created by michele on 08.01.23.
//

#include "Middleware.h"

namespace Espresso {

MiddlewareList::MiddlewareList() = default;

MiddlewareList::~MiddlewareList() = default;

void MiddlewareList::use(const Middleware &middleware) {
  this->middlewares_.push_back(middleware);
}

void MiddlewareList::run(HTTPRequest *request, HTTPResponse *response) {
  std::function<void()> next = []() {};
  for (auto it = this->middlewares_.begin(); it != this->middlewares_.end();
       ++it) {
    auto current = *it;
    next = [current, request, response, next]() {
      current(request, response, next);
    };
    next();
  }
}

} // Espresso