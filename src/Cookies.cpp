//
// Created by michele on 09.01.23.
//

#include "Cookies.h"

namespace Espresso {

Cookie::Cookie() = default;

Cookie::Cookie(std::string name, std::string value)
    : name(std::move(name)), value(std::move(value)) {}

Cookie::Cookie(std::string name,
               std::string value,
               std::string domain,
               std::string path,
               std::string expires,
               std::string max_age,
               bool secure,
               bool http_only) : name(std::move(name)),
                                 value(std::move(value)),
                                 domain(std::move(domain)),
                                 path(std::move(path)),
                                 expires(std::move(expires)),
                                 max_age(std::move(max_age)),
                                 secure(secure),
                                 http_only(http_only) {}

std::string Cookie::toString() const {
  std::string cookie_string = this->name + "=" + this->value;
  if (!this->domain.empty()) {
    cookie_string += "; Domain=" + this->domain;
  }
  if (!this->path.empty()) {
    cookie_string += "; Path=" + this->path;
  }
  if (!this->expires.empty()) {
    cookie_string += "; Expires=" + this->expires;
  }
  if (!this->max_age.empty()) {
    cookie_string += "; Max-Age=" + this->max_age;
  }
  if (this->secure) {
    cookie_string += "; Secure";
  }
  if (this->http_only) {
    cookie_string += "; HttpOnly";
  }
  return cookie_string;
}

} // Espresso