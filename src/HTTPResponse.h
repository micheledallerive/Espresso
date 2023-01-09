//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_HTTPRESPONSE_H_
#define ESPRESSO_SRC_HTTPRESPONSE_H_

#include <memory>
#include "HTTPMessage.h"
namespace Espresso {

class HTTPResponse;

class HTTPResponse : public HTTPMessage {
 public:
  HTTPResponse();
  virtual ~HTTPResponse();
  HTTPResponse *setStatus(int status);
  std::string toString();

  void sendFile(const std::string& path);
 private:
  int status_;
};

} // Espresso

#endif //ESPRESSO_SRC_HTTPRESPONSE_H_
