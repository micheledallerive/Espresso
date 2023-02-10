//
// Created by michele on 08.02.23.
//

#include "ModelField.h"

namespace Espresso::ORM {

template<class T>
void ModelField<T>::valueChanged() {
  // it changed only if it was already set
  if (this->set) this->dirty = true;
  // call valuechanged of super
  PrimitiveWrapper<T>::valueChanged();
}

} // ORM