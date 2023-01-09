//
// Created by michele on 08.01.23.
//

#include <sstream>
#include <utility>
#include <iostream>
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
  std::string method, path, version;
  std::string line;
  std::getline(iss, line);
  std::istringstream iss2(line);
  iss2 >> method >> path >> version;

  std::string queryString = path.substr(path.find('?') + 1);
  path = path.substr(0, path.find('?'));

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
  this->method_ = std::move(method);
  this->path_ = std::move(path);
  this->version_ = std::move(version);
  this->parseHeaders_(headers);
  this->parseQuery_(queryString);

  if (this->hasHeader("Cookie")) {
    this->parseCookies_(this->getHeader("Cookie"));
  }

  this->data["prova"] = std::string("tes");

  this->body_ = std::move(body);
}

HTTPRequest::~HTTPRequest() = default;

std::string HTTPRequest::getMethod() {
  return this->method_;
}

std::string HTTPRequest::getPath() {
  return this->path_;
}

void HTTPRequest::parseQuery_(const std::string &queryString) {
  std::istringstream iss(queryString);
  std::string key, value;
  while (std::getline(iss, key, '=')) {
    std::getline(iss, value, '&');
    this->query[key] = value;
  }
}

void HTTPRequest::parseCookies_(const std::string &cookiesString) {
  std::istringstream iss(cookiesString);
  std::string key, value;
  while (std::getline(iss, key, '=')) {
    std::getline(iss, value, ';');
    this->cookies[key] = value;
  }
}

} // Espresso