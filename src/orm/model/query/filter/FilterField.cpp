//
// Created by michele on 11.02.23.
//

#include "FilterField.h"
#include <vector>

namespace Espresso::ORM::Query {
std::string FilterField::toString() const {
  return value;
}
std::vector<std::string> FilterField::getKeys() const {
  return std::vector<std::string>{value};
}
bool FilterField::isTerminal() const {
  return true;
}

FilterOperation FilterField::operator==(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::EQUAL),
      new FilterField(right)};
}

FilterOperation FilterField::operator!=(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::NOT_EQUAL),
      new FilterField(right)};
}

FilterOperation FilterField::operator<(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::LESS_THAN),
      new FilterField(right)};
}

FilterOperation FilterField::operator<=(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::LESS_THAN_OR_EQUAL),
      new FilterField(right)};
}

FilterOperation FilterField::operator>(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::GREATER_THAN),
      new FilterField(right)};
}

FilterOperation FilterField::operator>=(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::GREATER_THAN_OR_EQUAL),
      new FilterField(right)};
}

FilterOperation FilterField::operator%(const std::string &right) const {
  return {new FilterField(*this),
      new RelationalOperator(RelationalOperator::LIKE),
      new FilterField(right)};
}
} // Query