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
  HTTPMessage();
  HTTPMessage(std::string version, const std::string& headers, std::string body);
  virtual ~HTTPMessage();

  void setHeader(const std::string& name, const std::string& value);
  std::string getHeader(const std::string& name);
  void setBody(std::string body);
  std::string getBody();
  std::string getVersion();

  std::string headersToString();

  void parseHeaders_(const std::string& headers);
 protected:
  std::string version_;
  std::unordered_map<std::string, std::string> headers_;
  std::string body_;
};

} // Espresso

#endif //ESPRESSO_LIB_HTTPMESSAGE_H_
