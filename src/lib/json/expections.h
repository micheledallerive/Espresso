//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_EXPECTIONS_H_
#define ESPRESSO_SRC_LIB_JSON_EXPECTIONS_H_

#include <exception>
#include <string>

namespace Espresso::JSON {
class JSONParseException : public std::exception {
 public:
  explicit JSONParseException(const std::string &message) : message_(message) {}
  const char *what() const noexcept override { return message_.c_str(); }
 private:
  std::string message_;
};
}

#endif //ESPRESSO_SRC_LIB_JSON_EXPECTIONS_H_
