//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_HTTPRESPONSE_H_
#define ESPRESSO_SRC_HTTPRESPONSE_H_

#include <memory>
#include "HTTPMessage.h"
#include "Cookies.h"

namespace Espresso {

class HTTPResponse;

class HTTPResponse : public HTTPMessage {
 public:
  HTTPResponse();
  ~HTTPResponse() override;
  std::string toString();

  HTTPResponse *setStatus(int status);
  void sendFile(const std::string &path);
  void send(const std::string &body);

  void setCookie(const Cookie &cookie);
  void setCookie(const std::string &name, const std::string &value);
  void deleteCookie(const std::string &name);
 private:
  int status_;
};

} // Espresso

#endif //ESPRESSO_SRC_HTTPRESPONSE_H_
