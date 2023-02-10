//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_

#include "ModelField.h"
namespace Espresso::ORM {

template<typename T>
class PrimaryKey : public ModelField<T> {
 public:
  PrimaryKey() = default;
  ~PrimaryKey() = default;

  PrimaryKey(const PrimaryKey<T> &other) = default;
  PrimaryKey(PrimaryKey<T> &&other) noexcept = default;

  virtual PrimaryKey<T> &operator=(const T &value) {
    ModelField<T>::operator=(value);
    return *this;
  }
  virtual PrimaryKey<T> &operator=(T &&value) noexcept {
    ModelField<T>::operator=(std::move(value));
    return *this;
  }
  PrimaryKey<T> &operator=(const PrimaryKey<T> &other) {
    ModelField<T>::operator=(other);
    return *this;
  }
  PrimaryKey<T> &operator=(PrimaryKey<T> &&other) noexcept {
    ModelField<T>::operator=(std::move(other));
    return *this;
  }

};

// write functions to check if a field is a primary key
template<typename T, typename U>
bool isPrimaryKey(const T U::*) {
  return false;
}

template<typename T, typename U>
bool isPrimaryKey(const PrimaryKey<T> U::*) {
  return true;
}

} // ORM

#include "PrimaryKey.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_
