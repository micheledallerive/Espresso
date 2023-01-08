//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_MIDDLEWARE_H_
#define ESPRESSO_SRC_MIDDLEWARE_H_

#include <functional>
#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace Espresso {

using Middleware = std::function<void(HTTPRequest *request,
                                      HTTPResponse *response,
                                      std::function<void()> next)>;

class MiddlewareList {
 public:
  MiddlewareList();
  virtual ~MiddlewareList();
  void use(const Middleware& middleware);
  void run(HTTPRequest *request, HTTPResponse *response);
 private:
  std::vector<Middleware> middlewares_;
};

} // Espresso


#endif //ESPRESSO_SRC_MIDDLEWARE_H_
