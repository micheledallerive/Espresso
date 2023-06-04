//
// Created by michele on 27.02.23.
//

#ifndef ESPRESSO_SRC_MIDDLEWARE_BASEMIDDLEWARE_H_
#define ESPRESSO_SRC_MIDDLEWARE_BASEMIDDLEWARE_H_
#include "requests/HTTPRequest.h"
#include "requests/HTTPResponse.h"
namespace Espresso {
class BaseMiddleware {
public:
  virtual void handle(HTTPRequest &request,
                      HTTPResponse &response,
                      const std::function<void()> &next) = 0;
};
}// namespace Espresso

#endif//ESPRESSO_SRC_MIDDLEWARE_BASEMIDDLEWARE_H_
