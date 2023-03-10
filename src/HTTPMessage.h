//
// Created by michele on 08.01.23.
//

#include<cstring>
#include<map>

#ifndef ESPRESSO_LIB_HTTPMESSAGE_H_
#define ESPRESSO_LIB_HTTPMESSAGE_H_

namespace Espresso {

class HTTPMessage {
 public:
  HTTPMessage();
  HTTPMessage(std::string version, const std::string& headers, std::string body);
  virtual ~HTTPMessage();

  void setHeader(const std::string& name, const std::string& value);
  bool hasHeader(const std::string& name);
  std::string getHeader(const std::string& name);

  void setBody(std::string body);
  std::string getBody();
  std::string getVersion();

  std::string headersToString();

  void parseHeaders_(const std::string& headers);
 protected:
  std::string version_;
  std::map<std::string, std::string> headers_;
  std::string body_;
};

} // Espresso

#endif //ESPRESSO_LIB_HTTPMESSAGE_H_
