//
// Created by michele on 27.02.23.
//

#ifndef ESPRESSO_SRC_MIDDLEWARE_FORMDATAMIDDLEWARE_H_
#define ESPRESSO_SRC_MIDDLEWARE_FORMDATAMIDDLEWARE_H_

#include <iostream>
#include "utils.h"
#include "BaseMiddleware.h"
namespace Espresso {

class FormDataMiddleware : public BaseMiddleware {
 public:
  void parseURLEncoded(HTTPRequest &request, HTTPResponse &response) {
    std::string rawBody = request.getRawBody();
    splitListOfPairs(rawBody, '&', '=', [&](const std::string &key,
                                            const std::string &value) {
      std::string decodedKey = key;
      std::replace(decodedKey.begin(), decodedKey.end(), '+', ' ');
      std::string decodedValue = value;
      std::replace(decodedValue.begin(), decodedValue.end(), '+', ' ');
      request.body[decodedKey] = decodedValue;
    });
  }

  void parseMultiPart(HTTPRequest &request, HTTPResponse &response) {
    std::string rawBody = request.getRawBody();

  }

  void handle(HTTPRequest &request, HTTPResponse &response,
              const std::function<void()> &next) override {
    if (request.hasHeader("Content-Type")) {
      if (request.getHeader("Content-Type").find("multipart/") !=
          std::string::npos) {
        parseMultiPart(request, response);
      } else if (request.getHeader("Content-Type")
          .find("application/x-www-form-urlencoded") != std::string::npos) {
        response.setRawBody(request.getRawBody());
        parseURLEncoded(request, response);
      }
    }
    next();
  }
};

}

#endif //ESPRESSO_SRC_MIDDLEWARE_FORMDATAMIDDLEWARE_H_
