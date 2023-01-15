//
// Created by michele on 08.01.23.
//

#include "Middleware.h"
#include "utils.h"
#include "PathRegex.h"

namespace Espresso {

MiddlewareList::MiddlewareList() = default;

MiddlewareList::~MiddlewareList() = default;

void MiddlewareList::use(const Middleware &middleware) {
  this->middlewares_.emplace_back(PathRegex::pathToRegex("/*"), middleware);
}

void MiddlewareList::use(const std::string &path,
                         const Middleware &middleware) {
  this->middlewares_.emplace_back(PathRegex::pathToRegex(path), middleware);
}

void MiddlewareList::run(HTTPRequest &request,
                         HTTPResponse &response) {
  // create a next function that, if called inside the middleware, will run the next one, otherwise will do nothing
  auto it = this->middlewares_.begin();
  std::function<void(void)> next = [&]() {
    if (it == this->middlewares_.end() || this->middlewares_.empty()) return;

    while (it != this->middlewares_.end()
        && !PathRegex::urlsMatch(it->first, request.getPath())) {
      ++it;
    }
    if (it == this->middlewares_.end()) return;

    auto [path, middleware] = *it;
    ++it;
    middleware(request, response, next);
  };
  next();
}

} // Espresso