//
// Created by michele on 08.01.23.
//

#include "Router.h"
#include "utils.h"

#include <utility>

namespace Espresso {
Router::Router() {
  this->routes_ = std::vector<Route>();
}
Router::~Router() = default;

void Router::get(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::GET, std::move(callback));
}

void Router::post(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::POST, std::move(callback));
}

void Router::put(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::PUT, std::move(callback));
}

void Router::del(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::DELETE, std::move(callback));
}

void Router::patch(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::PATCH, std::move(callback));
}

void Router::options(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::OPTIONS, std::move(callback));
}

void Router::head(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::HEAD, std::move(callback));
}

void Router::connect(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::CONNECT, std::move(callback));
}

void Router::trace(std::string path, RouteCallback callback) {
  this->addRoute(std::move(path), HTTPMethod::TRACE, std::move(callback));
}

void Router::addRoute(std::string path,
                      HTTPMethod method,
                      RouteCallback callback) {
  this->routes_.push_back(Route{std::move(path), method, std::move(callback)});
}

void Router::addRoute(std::vector<std::string> paths,
                      HTTPMethod method,
                      const RouteCallback &callback) {
  for (auto &path : paths) {
    this->addRoute(std::move(path), method, callback);
  }
}

void Router::executeMatchingRoute(Espresso::HTTPRequest *req,
                                  Espresso::HTTPResponse *res) {
  Route *matchingRoute = nullptr;
  const std::vector<std::string> &pathParts = split(req->getPath(), '/');
  std::vector<std::string> routePathParts;
  for (auto &route : this->routes_) {
    if (route.method != req->getMethod()) continue;
    if (urls_match(route.path, req->getPath())) {
      matchingRoute = &route;
      routePathParts = split(route.path, '/');
      break;
    }
  }
  if (matchingRoute) {
    for (int i = 0; i < routePathParts.size(); ++i) {
      const std::string routePathPart = routePathParts[i];
      if (routePathPart[0] == ':') {
        const std::string &paramValue = pathParts[i];
        req->params[routePathPart.substr(1)] = paramValue;
      }
    }
    matchingRoute->callback(req, res);
  }
}
void Router::route(const std::string& path,
                   const std::vector<std::pair<HTTPMethod, RouteCallback>>& callbacks) {
  for (auto &pair : callbacks) {
    this->addRoute(path, pair.first, pair.second);
  }
}

} // Espresso