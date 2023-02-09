//
// Created by michele on 09.02.23.
//

#include "BaseModelField.h"

namespace Espresso::ORM {

void BaseModelField::setDirty(bool d) {
  this->dirty = d;
}

} // ORM