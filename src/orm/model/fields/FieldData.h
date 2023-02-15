//
// Created by michele on 14.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_

#include "FieldParams.h"
#include "orm/sql/SQLTypes.h"
#include <sstream>
namespace Espresso::ORM {

class BaseFieldData : public FieldParams {
 public:
  std::string ctype;

  BaseFieldData() = default;
  virtual ~BaseFieldData() = default;
  bool operator==(const BaseFieldData &rhs) const {
    return name == rhs.name &&
        primaryKey == rhs.primaryKey &&
        autoIncrement == rhs.autoIncrement &&
        unique == rhs.unique &&
        notNull == rhs.notNull &&
        ((defaultValue.has_value() && rhs.defaultValue.has_value()
            && defaultValue.value() == rhs.defaultValue.value())
            || (!defaultValue.has_value() && !rhs.defaultValue.has_value())) &&
        ctype == rhs.ctype;
  }
  bool operator<(const BaseFieldData &rhs) const {
    return name < rhs.name;
  }

  [[nodiscard]] virtual std::string toSQL() const { return ""; }
};

class FieldData : public BaseFieldData {
 public:
  std::any field;

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
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_
