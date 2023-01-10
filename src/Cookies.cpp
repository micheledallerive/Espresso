//
// Created by michele on 09.01.23.
//

#include "Cookies.h"

#include <utility>

namespace Espresso {

Cookie::Cookie() = default;

Cookie::Cookie(std::string name, std::string value) : Cookie() {
  this->name = std::move(name);
  this->value = std::move(value);
}

Cookie::Cookie(std::string name,
               std::string value,
               std::string domain,
               std::string path,
               int max_age,
               bool secure,
               bool http_only) : Cookie() {
  this->name = std::move(name);
  this->value = std::move(value);
  this->domain = std::move(domain);
  this->path = std::move(path);
  this->max_age = max_age;
  this->secure = secure;
  this->http_only = http_only;
}

std::string Cookie::toString() const {
  if (this->name.empty() || this->value.empty()) {
    return "";
  }

  std::string cookie_string = this->name + "=" + this->value;
  if (!this->domain.empty()) {
    cookie_string += "; Domain=" + this->domain;
  }
  if (!this->path.empty()) {
    cookie_string += "; Path=" + this->path;
  }

  cookie_string += "; Max-Age=" + std::to_string(this->max_age);

  if (this->secure) {
    cookie_string += "; Secure";
  }
  if (this->http_only) {
    cookie_string += "; HttpOnly";
  }
  return cookie_string;
}

} // Espresso