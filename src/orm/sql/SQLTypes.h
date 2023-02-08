//
// Created by michele on 19.01.23.
//

#ifndef ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_
#define ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_

namespace Espresso::ORM {

enum class SQLType {
  INTEGER,
  REAL,
  TEXT,
  BLOB
};

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

struct SQLColumnInfo {
  std::string name;
  SQLType type{SQLType::BLOB};
  bool notNull{false};
  bool primaryKey{false};
  std::string defaultValue;

  bool operator==(const SQLColumnInfo &other) const {
    return name == other.name && type == other.type && notNull == other.notNull
        && primaryKey == other.primaryKey && defaultValue == other.defaultValue;
  }

  bool operator!=(const SQLColumnInfo &other) const {
    return !(*this == other);
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
};

}

#endif //ESPRESSO_SRC_ORM_SQL_SQLTYPES_H_
