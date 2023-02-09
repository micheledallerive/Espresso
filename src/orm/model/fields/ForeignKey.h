//
// Created by michele on 09.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_

#include <orm/model/fields/ModelField.h>

namespace Espresso::ORM {

template<class T>
class ForeignKey : public std::shared_ptr<ModelField<T>> {
};

} // ORM

#include "ForeignKey.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_
