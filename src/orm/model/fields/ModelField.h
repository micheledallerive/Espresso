//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_

#include "orm/utils/PrimitiveWrapper.h"
namespace Espresso::ORM {

template<typename T>
class ModelField : public PrimitiveWrapper<T> {
 public:
  typedef T value_type;
  ModelField() = default;
  virtual ~ModelField() = default;

  ModelField &operator=(const ModelField &wrapper) {
    this->value = wrapper.value;
    return *this;
  }
  ModelField &operator=(const T &value) {
    this->value = value;
    return *this;
  }
};

} // ORM
#include "ModelField.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_
