//
// Created by michele on 14.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_

#include "FieldParams.h"
#include "orm/sql/SQLTypes.h"
#include <orm/sql/SQLGenerator.h>
#include <sstream>
namespace Espresso::ORM {

class BaseFieldData {
 public:
  std::any field;
  std::string ctype;

  [[nodiscard]] virtual std::string toSQL() const = 0;
  [[nodiscard]] virtual SQLColumnInfo getColumnInfo() const = 0;
};

class FieldData : public FieldParams, public BaseFieldData {
 public:
  FieldData() = default;
  // create a constructor for the assignment ModelDataField = Field
  // Field does not have a constructor
  explicit FieldData(const FieldParams &f) {
    this->name = f.name;
    this->primaryKey = f.primaryKey;
    this->autoIncrement = f.autoIncrement;
    this->unique = f.unique;
    this->notNull = f.notNull;
    this->defaultValue = f.defaultValue;
  }

  [[nodiscard]] std::string toSQL() const override {
    std::stringstream ss;
    ss << this->name << " " << to_string(getSQLType(this->ctype));
    if (this->primaryKey) {
      ss << " PRIMARY KEY";
    }
    if (this->autoIncrement) {
      ss << " AUTOINCREMENT";
    }
    if (this->notNull) {
      ss << " NOT NULL";
    }
    if (!this->defaultValue.has_value()) {
      ss << " DEFAULT " << this->defaultValue.value();
    }
    return ss.str();
  }

  [[nodiscard]] SQLColumnInfo getColumnInfo() const override {
    SQLColumnInfo info;
    info.name = this->name;
    info.type = getSQLType(this->ctype);
    info.primaryKey = this->primaryKey;
    info.autoIncrement = this->autoIncrement;
    info.notNull = this->notNull;
    info.defaultValue = this->defaultValue;
    return info;
  }
};

class ForeignKeyFieldData : public FieldData {
 public:
  std::optional<ForeignKeyStruct_> foreignKey;

  ForeignKeyFieldData() = default;
  explicit ForeignKeyFieldData(const FieldParams &f) : FieldData(f) {}
  explicit ForeignKeyFieldData(const FieldData &f) : FieldData(f) {}

  [[nodiscard]] std::string toSQL() const override {
    std::stringstream ss;
    ss << FieldData::toSQL();
    if (this->foreignKey.has_value()) {
      ss << " REFERENCES " << this->foreignKey->table << "("
         << this->foreignKey->tablePrimaryKey << ")";
    }
    return ss.str();
  }

  [[nodiscard]] SQLColumnInfo getColumnInfo() const override {
    SQLColumnInfo info;
    info.name = this->name;
    info.type = getSQLType(this->ctype);
    info.primaryKey = this->primaryKey;
    info.autoIncrement = this->autoIncrement;
    info.unique = this->unique;
    info.notNull = this->notNull;
    info.defaultValue = this->defaultValue;
    info.foreignKey = this->foreignKey;
    return info;
  }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_
