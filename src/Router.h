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
#include "lib/trie/RouterTrie.h"

namespace Espresso {

class Router {
 public:
  Router();
  ~Router();

  void get(const std::string & path, RouteCallback callback);
  void post(const std::string & path, RouteCallback callback);
  void put(const std::string & path, RouteCallback callback);
  void del(const std::string & path, RouteCallback callback);
  void patch(const std::string & path, RouteCallback callback);
  void options(const std::string & path, RouteCallback callback);
  void head(const std::string & path, RouteCallback callback);
  void connect(const std::string & path, RouteCallback callback);
  void trace(const std::string & path, RouteCallback callback);
  void route(const std::string &path,
             const std::vector<std::pair<HTTPMethod,
                                         RouteCallback>> &callbacks);

  void addRoute(const std::string & path,
                HTTPMethod method,
                RouteCallback callback);

  void addRoute(const std::vector<std::string>& paths,
                HTTPMethod method,
                const RouteCallback &callback);
  void executeMatchingRoute(Espresso::HTTPRequest *req,
                            Espresso::HTTPResponse *res);
 private:
  RouterTrie routes_{'/'};
};

} // Espresso

#endif //ESPRESSO_SRC_ROUTER_H_
