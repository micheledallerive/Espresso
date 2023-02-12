//
// Created by michele on 11.02.23.
//

#include "FilterField.h"
#include <vector>

namespace Espresso::ORM::Query {
std::string FilterField::toString() const {
  return value;
}

FilterOperation *FilterField::operator==(const std::string &right) const {
  return new FilterOperation(new FilterField(*this),
                             new RelationalOperator(RelationalOperator::EQUAL),
                             new FilterField(right));
}
std::vector<std::string> FilterField::getKeys() const {
  return std::vector<std::string>{value};
}
} // Query