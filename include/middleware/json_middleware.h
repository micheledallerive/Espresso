//
// Created by michele on 26.02.23.
//

#ifndef ESPRESSO_SRC_MIDDLEWARE_JSONMIDDLEWARE_H_
#define ESPRESSO_SRC_MIDDLEWARE_JSONMIDDLEWARE_H_

#include "middleware.h"
#include "lib/json/json_entity.h"

namespace Espresso {

MiddlewareFunc JSONMiddleware = [](HTTPRequest &req, HTTPResponse &res, auto next) {
  if (req.hasHeader("Content-Type") && req.getHeader("Content-Type") == "application/json") {
    req.data["json"] = JSON::JSONEntity::parse(req.getBody());
  }
  next();
};

}

#endif//ESPRESSO_SRC_MIDDLEWARE_JSONMIDDLEWARE_H_
