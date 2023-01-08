//
// Created by michele on 08.01.23.
//

#include<cstring>
#include<unordered_map>

#ifndef ESPRESSO_LIB_HTTPMESSAGE_H_
#define ESPRESSO_LIB_HTTPMESSAGE_H_

namespace Espresso {

class HTTPMessage {
 public:
  HTTPMessage(std::string version, const std::string& headers, std::string body);
  virtual ~HTTPMessage();

  void setHeader(const char *name, const char *value);
  const char *getHeader(const char *name);
  void setBody(const char *body);
  std::string getBody();

  void parseHeaders_(const std::string& headers);
 private:
  std::string version_;
  std::unordered_map<std::string, std::string> headers_;
  std::string body_;
};

} // Espresso

#endif //ESPRESSO_LIB_HTTPMESSAGE_H_
