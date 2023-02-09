//
// Created by michele on 10.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELD_H_

#include <string>
#include <any>

namespace Espresso::ORM {

struct Field {
  std::string name{};
  bool primaryKey{false};
  bool autoIncrement{false};
  bool notNull{false};
  std::string defaultValue{};
};

class ModelDataField : public Field {
 public:
  std::any field;
  std::string ctype;

  ModelDataField() = default;
  // create a constructor for the assignment ModelDataField = Field
  // Field does not have a constructor
  explicit ModelDataField(const Field &f) {
    this->name = f.name;
    this->primaryKey = f.primaryKey;
    this->autoIncrement = f.autoIncrement;
    this->notNull = f.notNull;
    this->defaultValue = f.defaultValue;
  }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELD_H_
