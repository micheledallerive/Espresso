//
// Created by michele on 11.02.23.
//

#include "FilterField.h"

namespace Espresso::ORM::Query {
std::string FilterField::toString() const {
  return value;
}

FilterOperation *FilterField::operator==(const std::string &right) const {
  return new FilterOperation(new FilterField(*this),
                             Operation::EQUAL,
                             new FilterField(right));
}
} // Query