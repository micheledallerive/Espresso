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
  // create a next function that, if called inside the middleware, will run the next one, otherwise will do nothing
  auto it = this->middlewares_.begin();
  std::function<void(void)> next = [&]() {
    if (this->middlewares_.empty()) return;

    Middleware middleware = *it;
    ++it;
    middleware(request, response, next);
  };
  next();
}

} // Espresso