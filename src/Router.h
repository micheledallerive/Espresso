//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_ROUTER_H_
#define ESPRESSO_SRC_ROUTER_H_

#include <string>
#include <functional>
#include <vector>

#include "HTTPMethod.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace Espresso {

using RouteCallback = std::function<void(HTTPRequest *request, HTTPResponse *response)>;

struct Route {
  std::string path;
  HTTPMethod method;
  RouteCallback callback;
};

class Router {
 public:
  Router();
  ~Router();

  void get(std::string path, RouteCallback callback);
  void post(std::string path, RouteCallback callback);
  void put(std::string path, RouteCallback callback);
  void del(std::string path, RouteCallback callback);
  void patch(std::string path, RouteCallback callback);
  void options(std::string path, RouteCallback callback);
  void head(std::string path, RouteCallback callback);
  void connect(std::string path, RouteCallback callback);
  void trace(std::string path, RouteCallback callback);
  void addRoute(std::string path,
                HTTPMethod method,
                RouteCallback callback);

  Route *matchRoute(const std::string &path);
 private:
  std::vector<Route> routes_;
};

} // Espresso

#endif //ESPRESSO_SRC_ROUTER_H_
