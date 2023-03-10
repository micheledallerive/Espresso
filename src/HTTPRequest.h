//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_HTTPREQUEST_H_
#define ESPRESSO_SRC_HTTPREQUEST_H_

#include <unordered_map>
#include <any>
#include "HTTPMessage.h"
#include "HTTPMethod.h"
namespace Espresso {

class HTTPRequest : public HTTPMessage{
 public:
  HTTPRequest(HTTPMethod method, std::string path, std::string version, const std::string& headers, std::string body);
  explicit HTTPRequest(const std::string& request);
  ~HTTPRequest() override;

  HTTPMethod getMethod();
  std::string getPath();
  std::unordered_map<std::string, std::string> params;
  std::unordered_map<std::string, std::string> query;
  std::unordered_map<std::string, std::string> cookies;
  std::unordered_map<std::string, std::any> data;
 private:
  HTTPMethod method_;
  std::string path_;
  void parseQuery_(const std::string& queryString);
  void parseCookies_(const std::string& cookiesString);
};

} // Espresso

#endif //ESPRESSO_SRC_HTTPREQUEST_H_
