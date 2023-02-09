//
// Created by michele on 08.02.23.
//

#include "ModelField.h"

namespace Espresso::ORM {

template<class T>
void ModelField<T>::valueChanged() {
  this->dirty = true;
}

} // ORM