//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_HTTPREQUEST_H_
#define ESPRESSO_SRC_HTTPREQUEST_H_

#include "HTTPMessage.h"
namespace Espresso {

class HTTPRequest : public HTTPMessage{
 public:
  HTTPRequest(std::string method, std::string path, std::string version, const std::string& headers, std::string body);
  explicit HTTPRequest(const std::string& request);
  virtual ~HTTPRequest();

  std::string getMethod();
  std::string getPath();
 private:
  std::string method_;
  std::string path_;
};

} // Espresso

#endif //ESPRESSO_SRC_HTTPREQUEST_H_
