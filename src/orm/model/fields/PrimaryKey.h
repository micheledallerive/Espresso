//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_

#include "ModelField.h"
namespace Espresso::ORM {

template<typename T>
class PrimaryKey : public ModelField<T> {

};

// write functions to check if a field is a primary key
template<typename T, typename U>
bool isPrimaryKey(const ModelField<T> U::*field) {
  return false;
}

template<typename T, typename U>
bool isPrimaryKey(const PrimaryKey<T> U::*field) {
  return true;
}

} // ORM

#include "PrimaryKey.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_
