//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_

namespace Espresso::ORM {

template<typename T>
class ModelField {
 public:
  ModelField() = default;
  virtual ~ModelField() = default;

};

} // ORM
#include "ModelField.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_
