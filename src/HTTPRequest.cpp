//
// Created by michele on 08.01.23.
//

#include <sstream>
#include <utility>
#include <iostream>

#include "HTTPRequest.h"
#include "HTTPMethod.h"
#include "utils.h"

namespace Espresso {

HTTPRequest::HTTPRequest(HTTPMethod method,
                         std::string path,
                         std::string version,
                         const std::string &headers,
                         std::string body) : HTTPMessage(std::move(version),
                                                         headers,
                                                         std::move(body)) {
  this->method_ = method;
  this->path_ = std::move(path);
}

HTTPRequest::HTTPRequest(const std::string &request) {
  std::istringstream iss(request);
  std::string method, path, version;
  std::string line;
  std::getline(iss, line);
  std::istringstream iss2(line);
  iss2 >> method >> path >> version;

  auto queryPos = path.find('?');
  if (queryPos != std::string::npos) {
    this->parseQuery_(path.substr(queryPos + 1));
    path = path.substr(0, queryPos);
  }

  std::string headers;
  while (std::getline(iss, line)) {
    if (line == "\r") {
      break;
    }
    headers += line + "\n";
  }
  std::string body;
  while (std::getline(iss, line)) {
    body += line;
  }
  this->method_ = stringToMethod(std::move(method));
  this->path_ = std::move(path);
  this->version_ = std::move(version);
  this->parseHeaders_(headers);

  if (this->hasHeader("Cookie")) {
    this->parseCookies_(this->getHeader("Cookie"));
  }

  this->data["prova"] = std::string("tes");

  this->body_ = std::move(body);
}

HTTPRequest::~HTTPRequest() = default;

HTTPMethod HTTPRequest::getMethod() {
  return this->method_;
}

std::string HTTPRequest::getPath() {
  return this->path_;
}

void HTTPRequest::parseQuery_(const std::string &queryString) {
  splitListOfPairs(queryString,
                   '&',
                   '=',
                   [&](const std::string &key, const std::string &value) {
                     if (!key.empty()) {
                       this->query[key] = value;
                     }
                   });
}

void HTTPRequest::parseCookies_(const std::string &cookiesString) {
  splitListOfPairs(" " + cookiesString,
                   ';',
                   '=',
                   [&](const std::string &key, const std::string &value) {
                     if (key.length() > 1) {
                       this->cookies[key.substr(1)] = value;
                     }
                   });
}

} // Espresso