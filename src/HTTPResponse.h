//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_HTTPRESPONSE_H_
#define ESPRESSO_SRC_HTTPRESPONSE_H_

#include <memory>
#include <unordered_map>
#include "HTTPMessage.h"
#include "Cookies.h"

namespace Espresso {

extern std::unordered_map<int, std::string> HTTP_REASONS;

class HTTPResponse : public HTTPMessage {
 public:
  HTTPResponse();
  ~HTTPResponse() override;
  std::string toString();

  HTTPResponse &setStatus(int status);
  [[nodiscard]] int getStatus() const;

  void send(const std::string &body);
  void sendFile(const std::string &path);
  void downloadFile(const std::string &path, const std::string &filename = "");

  void redirect(const std::string &path, int status = 302);

  void setCookie(const Cookie &cookie);
  void setCookie(const std::string &name, const std::string &value);
  void deleteCookie(const std::string &name);
 private:
  int status_;
  void addDefaultHeaders_();
  std::map<std::string, Cookie> cookies_;
};

} // Espresso

#endif //ESPRESSO_SRC_HTTPRESPONSE_H_
