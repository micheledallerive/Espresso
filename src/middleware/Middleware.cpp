//
// Created by michele on 08.01.23.
//

#include "middleware/Middleware.h"
#include "utils.h"
#include "routing/PathRegex.h"

namespace Espresso {

MiddlewareList::MiddlewareList() = default;

MiddlewareList::~MiddlewareList() = default;

void MiddlewareList::use(const MiddlewareFunc &middleware) {
  this->middlewares_.emplace_back(PathRegex::pathToRegex("/*"), middleware);
}

void MiddlewareList::use(const std::string &path,
                         const MiddlewareFunc &middleware) {
  this->middlewares_.emplace_back(PathRegex::pathToRegex(path), middleware);
}

void MiddlewareList::use(BaseMiddleware &middleware) {
  this->use([&](auto &req, auto &res, const auto &next) {
    middleware.handle(req, res, next);
  });
}

void MiddlewareList::use(const std::string &path,
                         BaseMiddleware &middleware) {
  this->use(path, [&](auto &req, auto &res, const auto &next) {
    middleware.handle(req, res, next);
  });
}

void MiddlewareList::use(BaseMiddleware &&middleware) {
  this->use(middleware);
}

void MiddlewareList::use(const std::string &path,
                         BaseMiddleware &&middleware) {
  this->use(path, middleware);
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