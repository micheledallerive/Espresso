//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_ROUTER_H_
#define ESPRESSO_SRC_ROUTER_H_

#include <string>
#include <functional>
#include <list>

namespace Espresso {

struct Route {
  std::string path;
  std::function<void()> callback;
};

class Router {
 public:
  Router();
  ~Router();

  void addRoute(std::string path, std::function<void()> callback);
  void matchRoute(std::string path);
 private:
  std::list<Route> routes_;
};

} // Espresso

#endif //ESPRESSO_SRC_ROUTER_H_
