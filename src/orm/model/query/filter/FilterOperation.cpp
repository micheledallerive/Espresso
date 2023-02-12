//
// Created by michele on 11.02.23.
//

#include "FilterOperation.h"

namespace Espresso::ORM::Query {

std::string FilterOperation::toString() const {
  return left->toString() + " " + op->toString() + " "
      + right->toString();
}
} // Query