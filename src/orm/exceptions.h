//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_EXCEPTIONS_H_
#define ESPRESSO_SRC_ORM_EXCEPTIONS_H_

#include <stdexcept>

namespace Espresso {

class sql_error : public std::runtime_error {
 public:
  explicit sql_error(const std::string &message) : std::runtime_error(
      message) {}
};

class db_error : public std::runtime_error {
 public:
  explicit db_error(const std::string &message) : std::runtime_error(
      message) {}
};

}

#endif //ESPRESSO_SRC_ORM_EXCEPTIONS_H_
