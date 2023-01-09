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
               int max_age,
               bool secure,
               bool http_only) : name(std::move(name)),
                                 value(std::move(value)),
                                 domain(std::move(domain)),
                                 path(std::move(path)),
                                 max_age(max_age),
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