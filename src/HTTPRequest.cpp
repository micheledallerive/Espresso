//
// Created by michele on 08.01.23.
//

#include <sstream>
#include <utility>
#include "HTTPRequest.h"

namespace Espresso {
HTTPRequest::HTTPRequest(std::string method,
                         std::string path,
                         std::string version,
                         const std::string &headers,
                         std::string body) : HTTPMessage(std::move(version),
                                                         headers,
                                                         std::move(body)) {
  this->method_ = std::move(method);
  this->path_ = std::move(path);
}

HTTPRequest::HTTPRequest(const std::string &request) {
  std::istringstream iss(request);
  std::string line;
  std::getline(iss, line);
  std::string method = line.substr(0, line.find(' '));
  std::string path = line.substr(line.find(' ') + 1,
                                 line.find(' ', line.find(' ') + 1)
                                     - line.find(' ') - 1);
  std::string version = line.substr(line.find(' ', line.find(' ') + 1) + 1);
  version.pop_back(); // remove the \r
  this->method_ = std::move(method);
  this->path_ = std::move(path);
  this->version_ = std::move(version);
  std::getline(iss, line);
  this->parseHeaders_(line);
  std::getline(iss, line);
  this->body_ = std::move(line);
}

HTTPRequest::~HTTPRequest() = default;

} // Espresso