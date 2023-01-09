//
// Created by michele on 09.01.23.
//

#include "Cookies.h"

namespace Espresso {

struct Cookie {
  std::string name;
  std::string value;
  std::string domain;
  std::string path;
  std::string expires;
  std::string max_age;
  bool secure;
  bool http_only;
};

std::string to_string(struct Cookie &cookie) {
  std::string cookie_string = cookie.name + "=" + cookie.value;
  if (!cookie.domain.empty()) {
    cookie_string += "; Domain=" + cookie.domain;
  }
  if (!cookie.path.empty()) {
    cookie_string += "; Path=" + cookie.path;
  }
  if (!cookie.expires.empty()) {
    cookie_string += "; Expires=" + cookie.expires;
  }
  if (!cookie.max_age.empty()) {
    cookie_string += "; Max-Age=" + cookie.max_age;
  }
  if (cookie.secure) {
    cookie_string += "; Secure";
  }
  if (cookie.http_only) {
    cookie_string += "; HttpOnly";
  }
  return cookie_string;
}

} // Espresso