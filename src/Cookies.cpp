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

} // Espresso