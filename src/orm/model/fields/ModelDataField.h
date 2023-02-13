//
// Created by michele on 14.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELDATAFIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELDATAFIELD_H_

#include "Field.h"
namespace Espresso::ORM {

class ModelDataField : public Field {
 public:
  std::any field;
  std::string ctype;
  std::optional<ForeignKeyData> foreignKey;

  ModelDataField() = default;
  // create a constructor for the assignment ModelDataField = Field
  // Field does not have a constructor
  explicit ModelDataField(const Field &f) {
    this->name = f.name;
    this->primaryKey = f.primaryKey;
    this->autoIncrement = f.autoIncrement;
    this->notNull = f.notNull;
    this->defaultValue = f.defaultValue;
    this->foreignKey = std::nullopt;
  }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELDATAFIELD_H_
