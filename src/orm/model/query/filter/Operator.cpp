//
// Created by michele on 12.02.23.
//

#include "Operator.h"

namespace Espresso::ORM::Query {
std::string LogicalOperator::toString() const {
  switch (op) {
    case AND:return "AND";
    case OR:return "OR";
    case NOT:return "NOT";
    default:return "";
  }
}
std::string RelationalOperator::toString() const {
  switch (op) {
    case EQUAL:return "=";
    case NOT_EQUAL:return "!=";
    case GREATER_THAN:return ">";
    case GREATER_THAN_OR_EQUAL:return ">=";
    case LESS_THAN:return "<";
    case LESS_THAN_OR_EQUAL:return "<=";
    case LIKE:return "LIKE";
    default:return "";
  }
}
} // Query