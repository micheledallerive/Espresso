//
// Created by michele on 19.01.23.
//

#ifndef ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_
#define ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_

#include <unordered_map>
#include <string>
#include <utility>
#include <orm/model/fields/FieldParams.h>

namespace Espresso::ORM {

using ConstraintMap = std::unordered_map<std::string, std::string>;

enum class SQLType {
  INTEGER,
  REAL,
  TEXT,
  BLOB
};

std::string to_string(SQLType type);

SQLType getSQLType(const std::string &ctype);

}

#endif //ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_
