//
// Created by michele on 08.01.23.
//

#include "HTTPMessage.h"

#include <utility>
#include <iostream>
#include <sstream>

namespace Espresso {

HTTPMessage::HTTPMessage() = default;

HTTPMessage::HTTPMessage(std::string version,
                         const std::string &headers,
                         std::string body) {
  this->version_ = std::move(version);
  this->parseHeaders_(headers);
  this->body_ = std::move(body);
}

HTTPMessage::~HTTPMessage() = default;

void HTTPMessage::parseHeaders_(const std::string &headers) {
  std::istringstream iss(headers);
  std::string line;
  while (std::getline(iss, line)) {
    const auto pos = line.find(':');
    if (pos == std::string::npos) continue; // invalid header
    std::string name = line.substr(0, pos);
    std::string value;
    if (pos + 2 >= line.length())
      value = ""; // empty header
    else {
      value = line.substr(pos + 2);
      value.pop_back(); // remove the \r
    }
    this->headers_.insert({name, value});
  }
}

void HTTPMessage::setHeader(const std::string &name, const std::string &value) {
  this->headers_[name] = value;
}

bool HTTPMessage::hasHeader(const std::string &name) {
  return this->headers_.find(name) != this->headers_.end();
}

std::string HTTPMessage::getHeader(const std::string &name) {
  return this->headers_.at(name);
}

void HTTPMessage::setBody(std::string body) {
  this->body_ = std::move(body);
}

std::string HTTPMessage::getBody() {
  return this->body_;
}

std::string HTTPMessage::getVersion() {
  return this->version_;
}

std::string HTTPMessage::headersToString() {
  std::string headers;
  for (auto &header : this->headers_) {
    headers += header.first + ": " + header.second + "\r\n";
  }
  return headers;
}

} // Espresso