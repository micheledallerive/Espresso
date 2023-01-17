//
// Created by michele on 08.01.23.
//

#include "Router.h"
#include "PathRegex.h"
#include "utils.h"

#include <utility>

namespace Espresso {
Router::Router() = default;
Router::~Router() = default;

void Router::get(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::GET, std::move(callback));
}

void Router::post(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::POST, std::move(callback));
}

void Router::put(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::PUT, std::move(callback));
}

void Router::del(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::DELETE, std::move(callback));
}

void Router::patch(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::PATCH, std::move(callback));
}

void Router::options(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::OPTIONS, std::move(callback));
}

void Router::head(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::HEAD, std::move(callback));
}

void Router::connect(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::CONNECT, std::move(callback));
}

void Router::trace(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::TRACE, std::move(callback));
}

void Router::all(const std::string &path, RouteCallback callback) {
  this->addRoute((path), HTTPMethod::ALL, std::move(callback));
}

void Router::addRoute(const std::string &path,
                      HTTPMethod method,
                      RouteCallback callback) {
  this->routes_.push_back({path, {}, method, std::move(callback)});
}

void Router::addRoute(const std::vector<std::string> &paths,
                      HTTPMethod method,
                      const RouteCallback &callback) {
  for (auto &path : paths) {
    this->addRoute((path), method, callback);
  }
}

void Router::executeMatchingRoute(HTTPRequest &req,
                                  HTTPResponse &res) {
  for (auto &route : this->routes_) {
    if (req.getMethod() == route.method
        && PathRegex::urlsMatch(route.path, req.getPath())) {
      PathRegex::retrieveParams(route, req.getPath(), req.params);
      route.callback(req, res);
      return;
    }
  }
}

void Router::route(const std::string &path,
                   const std::vector<std::pair<HTTPMethod,
                                               RouteCallback>> &callbacks) {
  for (auto &pair : callbacks) {
    this->addRoute(path, pair.first, pair.second);
  }
}

} // Espresso