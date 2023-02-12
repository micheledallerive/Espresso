//
// Created by michele on 11.02.23.
//

#include "FilterOperation.h"
#include "Operation.h"

namespace Espresso::ORM::Query {

std::string FilterOperation::toString() const {
  return left->toString() + " " + op->toString() + " "
      + right->toString();
}
FilterOperation *FilterOperation::operator&(const FilterNode &other) const {
  return new FilterOperation((FilterNode *) this,
                             new LogicalOperator(LogicalOperator::AND),
                             (FilterNode *) &other);
}
FilterOperation *FilterOperation::operator|(const FilterNode &other) const {
  return new FilterOperation((FilterNode *) this,
                             new LogicalOperator(LogicalOperator::OR),
                             (FilterNode *) &other);
}
} // Query