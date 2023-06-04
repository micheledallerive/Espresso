//
// Created by michele on 08.01.23.
//

#include "requests/HTTPMessage.h"

#include <utility>
#include <iostream>
#include <sstream>
#include "utils.h"

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
  splitListOfPairs(headers,
                   '\n',
                   ':',
                   [&](const std::string &key, std::string value) {
                     if (value.back() == '\r') value.pop_back();
                     if (value.front() == ' ') value.erase(0, 1);
                     if (!key.empty() && !value.empty()) {
                       this->headers_[key] = value;
                     }
                   });
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