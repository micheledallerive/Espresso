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

class SQLColumnInfo;

class SQLColumnInfo : public FieldParams {
 public:
  SQLType type{SQLType::BLOB};
  std::optional<ForeignKeyStruct_> foreignKey;

  bool operator==(const SQLColumnInfo &other) const {
    return name == other.name;
  }

  [[nodiscard]] bool equals(const SQLColumnInfo &other) const {
    return name == other.name && type == other.type && notNull == other.notNull
        &&
            primaryKey == other.primaryKey
        && defaultValue == other.defaultValue;
  }

  bool operator!=(const SQLColumnInfo &other) const {
    return !(*this == other);
  }

  bool operator<(const SQLColumnInfo &other) const {
    return name < other.name;
  }

  explicit SQLColumnInfo(const std::unordered_map<std::string,
                                                  std::string> &data) {
    name = data.at("name");
    if (data.at("type") == "INTEGER") {
      type = SQLType::INTEGER;
    } else if (data.at("type") == "REAL") {
      type = SQLType::REAL;
    } else if (data.at("type") == "TEXT") {
      type = SQLType::TEXT;
    } else if (data.at("type") == "BLOB") {
      type = SQLType::BLOB;
    }
    notNull = data.at("notnull") == "1";
    primaryKey = data.at("pk") == "1";
    defaultValue = data.at("dflt_value");
  }

  SQLColumnInfo() = default;
  SQLColumnInfo(SQLColumnInfo &&) = default;
  SQLColumnInfo(const SQLColumnInfo &) = default;

  SQLColumnInfo &operator=(const SQLColumnInfo &) = default;
  SQLColumnInfo &operator=(SQLColumnInfo &&) = default;
};

}

#endif //ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_
