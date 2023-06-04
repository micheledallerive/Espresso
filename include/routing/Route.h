//
// Created by michele on 15.01.23.
//

#ifndef ESPRESSO_SRC_ROUTE_H_
#define ESPRESSO_SRC_ROUTE_H_

#include <functional>
#include <string>
#include "requests/HTTPRequest.h"
#include "requests/HTTPResponse.h"

namespace Espresso {

using RouteCallback = std::function<void(HTTPRequest &request,
                                         HTTPResponse &response)>;

class Route {
 public:
  Route(const std::string &path, HTTPMethod method, RouteCallback callback);
  ~Route();

  std::string path{"/"};
  std::vector<std::string> params;
  HTTPMethod method{HTTPMethod::GET};
  RouteCallback callback;
};

}
#endif //ESPRESSO_SRC_ROUTE_H_
