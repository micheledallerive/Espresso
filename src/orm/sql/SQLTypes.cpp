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

SQLType getSQLType(const std::string &type) {
  if (type == typeid(int).name()) {
    return SQLType::INTEGER;
  } else if (type == typeid(float).name()) {
    return SQLType::REAL;
  } else if (type == typeid(double).name()) {
    return SQLType::REAL;
  } else if (type == typeid(std::string).name()) {
    return SQLType::TEXT;
  } else if (type == typeid(bool).name()) {
    return SQLType::INTEGER;
  } else {
    return SQLType::BLOB;
  }
}
}