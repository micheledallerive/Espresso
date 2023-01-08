//
// Created by michele on 08.01.23.
//

#include "HTTPMessage.h"

#include <utility>
#include <iostream>
#include <sstream>

namespace Espresso {

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
    std::string name = line.substr(0, line.find(':'));
    std::string value = line.substr(line.find(':') + 2);
    value.pop_back(); // remove the \r
    this->headers_.insert({name, value});
  }
}

void HTTPMessage::setHeader(const std::string &name, const std::string &value) {
  this->headers_.insert({name, value});
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

} // Espresso