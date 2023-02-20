//
// Created by michele on 20.02.23.
//

#ifndef ESPRESSO_SRC_ORM_UTILS_SQLIZABLE_H_
#define ESPRESSO_SRC_ORM_UTILS_SQLIZABLE_H_

#include <string>

class SQLizable {
 public:
  virtual std::string toSQL() const = 0;
};

#endif //ESPRESSO_SRC_ORM_UTILS_SQLIZABLE_H_
