//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_

#include <orm/utils/PrimitiveWrapper.h>
#include "BaseModelField.h"
namespace Espresso::ORM {

template<typename T>
class ModelField : public PrimitiveWrapper<T>, public BaseModelField {
 protected:
  void valueChanged() override;
 public:
  typedef T value_type;
  ModelField() = default;
  ModelField(const ModelField &wrapper)
      : PrimitiveWrapper<T>(wrapper), BaseModelField(wrapper) {}
  explicit ModelField(T val) : PrimitiveWrapper<T>(val), BaseModelField() {}
  ~ModelField() override = default;

  ModelField &operator=(const ModelField &wrapper) {
    // use super class operator
    PrimitiveWrapper<T>::operator=(wrapper);
    return *this;
  }
  ModelField &operator=(const T &value) override {
    // use super class operator
    PrimitiveWrapper<T>::operator=(value);
    return *this;
  }
};

} // ORM
#include "ModelField.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_MODELFIELD_H_
