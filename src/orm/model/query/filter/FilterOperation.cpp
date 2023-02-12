//
// Created by michele on 11.02.23.
//

#include "FilterOperation.h"
#include "Operator.h"
#include <vector>

namespace Espresso::ORM::Query {

std::string FilterOperation::toString() const {
  return left->toString() + " "
      + op->toString() + " "
      + (right->isTerminal() ? "'" + right->toString() + "'" :
          right->toString());
}
FilterOperation FilterOperation::operator&(const FilterOperation &other) const {
  return {new FilterOperation(*this),
      new LogicalOperator(LogicalOperator::AND),
      new FilterOperation(other)};
}
FilterOperation *FilterOperation::operator|(const FilterNode &other) const {
  return new FilterOperation((FilterNode *) this,
                             new LogicalOperator(LogicalOperator::OR),
                             (FilterNode *) &other);
}
std::vector<std::string> FilterOperation::getKeys() const {
  std::vector<std::string> keys;
  std::vector<std::string> leftKeys = left->getKeys();
  keys.insert(keys.end(), leftKeys.begin(), leftKeys.end());
  return keys;
}
bool FilterOperation::isTerminal() const {
  return false;
}

} // Query