//
// Created by michele on 08.02.23.
//
#include <string>
#include "SQLTypes.h"

namespace Espresso::ORM {
std::string to_string(SQLType type) {
  switch (type) {
    case SQLType::INTEGER:return "INTEGER";
    case SQLType::REAL:return "REAL";
    case SQLType::TEXT:return "TEXT";
    case SQLType::BLOB:return "BLOB";
    default: return "";
  }
}

SQLType getSQLType(const std::string &ctype) {
  if (ctype == typeid(int).name()) {
    return SQLType::INTEGER;
  } else if (ctype == typeid(float).name()) {
    return SQLType::REAL;
  } else if (ctype == typeid(double).name()) {
    return SQLType::REAL;
  } else if (ctype == typeid(std::string).name()) {
    return SQLType::TEXT;
  } else if (ctype == typeid(bool).name()) {
    return SQLType::INTEGER;
  } else {
    return SQLType::BLOB;
  }
}
}