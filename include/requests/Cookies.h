//
// Created by michele on 09.01.23.
//

#ifndef ESPRESSO_SRC_COOKIES_H_
#define ESPRESSO_SRC_COOKIES_H_

#include <string>

namespace Espresso {

class Cookie {
public:
  Cookie();
  Cookie(std::string name, std::string value);
  Cookie(std::string name,
         std::string value,
         std::string domain,
         std::string path,
         int max_age,
         bool secure,
         bool http_only);
  std::string name;
  std::string value;
  std::string domain;
  std::string path = "/";
  int max_age = 60 * 60;
  bool secure{};
  bool http_only{};

  [[nodiscard]] std::string toString() const;
};

}// namespace Espresso

#endif//ESPRESSO_SRC_COOKIES_H_
