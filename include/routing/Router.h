//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_ROUTER_H_
#define ESPRESSO_SRC_ROUTER_H_

#include <functional>
#include <string>
#include <vector>

#include "Route.h"
#include "requests/HTTPMethod.h"
#include "requests/HTTPRequest.h"
#include "requests/HTTPResponse.h"

namespace Espresso {

class Router {
public:
  Router();
  ~Router();

  void get(const std::string &path, RouteCallback callback);
  void post(const std::string &path, RouteCallback callback);
  void put(const std::string &path, RouteCallback callback);
  void del(const std::string &path, RouteCallback callback);
  void patch(const std::string &path, RouteCallback callback);
  void options(const std::string &path, RouteCallback callback);
  void head(const std::string &path, RouteCallback callback);
  void connect(const std::string &path, RouteCallback callback);
  void trace(const std::string &path, RouteCallback callback);
  void route(const std::string &path,
             const std::vector<std::pair<HTTPMethod,
                                         RouteCallback>> &callbacks);
  void all(const std::string &path, RouteCallback callback);

  void addRoute(const std::string &path,
                HTTPMethod method,
                RouteCallback callback);

  void addRoute(const std::vector<std::string> &paths,
                HTTPMethod method,
                const RouteCallback &callback);
  void executeMatchingRoute(HTTPRequest &req,
                            HTTPResponse &res);

private:
  std::vector<Route> routes_;
};

}// namespace Espresso

#endif//ESPRESSO_SRC_ROUTER_H_
