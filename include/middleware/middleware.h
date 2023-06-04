//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_MIDDLEWARE_H_
#define ESPRESSO_SRC_MIDDLEWARE_H_

#include "middleware/base_middleware.h"
#include "requests/http_request.h"
#include "requests/http_response.h"
#include <functional>

namespace Espresso {

using MiddlewareFunc = std::function<void(HTTPRequest &request,
                                          HTTPResponse &response,
                                          const std::function<void()> &next)>;

class MiddlewareList {
public:
  MiddlewareList();
  virtual ~MiddlewareList();
  void use(const MiddlewareFunc &middleware);
  void use(const std::string &path, const MiddlewareFunc &middleware);
  void use(BaseMiddleware &middleware);
  void use(const std::string &path, BaseMiddleware &middleware);
  void use(BaseMiddleware &&middleware);
  void use(const std::string &path, BaseMiddleware &&middleware);

  void run(HTTPRequest &request, HTTPResponse &response);

private:
  std::vector<std::pair<std::string, MiddlewareFunc>> middlewares_;
};

}// namespace Espresso

#endif//ESPRESSO_SRC_MIDDLEWARE_H_
