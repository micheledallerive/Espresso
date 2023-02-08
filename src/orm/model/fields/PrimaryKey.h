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

} // ORM

#include "PrimaryKey.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_PRIMARYKEY_H_
