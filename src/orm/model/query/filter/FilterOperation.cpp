//
// Created by michele on 11.02.23.
//

#include "FilterOperation.h"
#include "Operator.h"
#include <vector>
#include <sstream>

namespace Espresso::ORM::Query {

std::string FilterOperation::toString() const {
  std::ostringstream sql;
  if (left != nullptr) {
    if (left->isTerminal()) {
      sql << left->toString();
    } else {
      sql << "(" << left->toString() << ")";
    }
    sql << " ";
  }
  sql << op->toString();
  if (right != nullptr) {
    sql << " ";
    if (right->isTerminal()) {
      sql << right->toString();
    } else {
      sql << "(" << right->toString() << ")";
    }
  }
  return sql.str();
}
FilterOperation FilterOperation::operator&(const FilterOperation &other) const {
  return {new FilterOperation(*this),
      new LogicalOperator(LogicalOperator::AND),
      new FilterOperation(other)};
}
FilterOperation FilterOperation::operator|(const FilterOperation &other) const {
  return {new FilterOperation(*this),
      new LogicalOperator(LogicalOperator::OR),
      new FilterOperation(other)};
}
FilterOperation FilterOperation::operator!() const {
  return {nullptr,
      new LogicalOperator(LogicalOperator::NOT),
      new FilterOperation(*this)};
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