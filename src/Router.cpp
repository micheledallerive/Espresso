//
// Created by michele on 08.01.23.
//

#include "Router.h"

#include <utility>

namespace Espresso {
Router::Router() {
  this->routes_ = std::vector<Route>();
}
Router::~Router() = default;

void Router::addRoute(std::string path,
                      HTTPMethod method,
                      RouteCallback callback) {
  this->routes_.push_back(Route{std::move(path), method, std::move(callback)});
}

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

Route *Router::matchRoute(const std::string &path) {
  for (auto &route : this->routes_) {
    if (route.path == path) {
      return &route;
    }
  }
  return nullptr;
}

} // Espresso