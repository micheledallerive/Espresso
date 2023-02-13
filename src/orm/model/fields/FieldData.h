//
// Created by michele on 14.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_

#include "FieldParams.h"
namespace Espresso::ORM {

class FieldData : public FieldParams {
 public:
  std::any field;
  std::string ctype;
  std::optional<ForeignKeyData> foreignKey;

  FieldData() = default;
  // create a constructor for the assignment ModelDataField = Field
  // Field does not have a constructor
  explicit FieldData(const FieldParams &f) {
    this->name = f.name;
    this->primaryKey = f.primaryKey;
    this->autoIncrement = f.autoIncrement;
    this->notNull = f.notNull;
    this->defaultValue = f.defaultValue;
    this->foreignKey = std::nullopt;
  }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDDATA_H_
