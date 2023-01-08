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

struct Route {
  std::string path;
  HTTPMethod method;
  std::function<void(HTTPRequest *, HTTPResponse *)> callback;
};

class Router {
 public:
  Router();
  ~Router();

  void get(std::string path, std::function<void()> callback);
  void post(std::string path, std::function<void()> callback);
  void put(std::string path, std::function<void()> callback);
  void del(std::string path, std::function<void()> callback);
  void patch(std::string path, std::function<void()> callback);
  void options(std::string path, std::function<void()> callback);
  void head(std::string path, std::function<void()> callback);
  void connect(std::string path, std::function<void()> callback);
  void trace(std::string path, std::function<void()> callback);
  void addRoute(std::string path,
                HTTPMethod method,
                std::function<void()> callback);

  Route *matchRoute(const std::string &path);
 private:
  std::vector<Route> routes_;
};

} // Espresso

#endif //ESPRESSO_SRC_ROUTER_H_
