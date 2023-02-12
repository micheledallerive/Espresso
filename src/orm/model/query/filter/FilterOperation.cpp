//
// Created by michele on 11.02.23.
//

#include "FilterOperation.h"

namespace Espresso::ORM::Query {

std::string operatorToString(Operation op) {
  switch (op) {
    case Operation::EQUAL:return "=";
    case Operation::NOT_EQUAL:return "!=";
    case Operation::GREATER:return ">";
    case Operation::GREATER_EQUAL:return ">=";
    case Operation::LESS:return "<";
    case Operation::LESS_EQUAL:return "<=";
    default: return "";
  }
}

std::string FilterOperation::toString() const {
  return left->toString() + " " + operatorToString(op) + " "
      + right->toString();
}
} // Query