//
// Created by michele on 24.02.23.
//

#include <iomanip>
#include "JSONNumber.h"

namespace Espresso::JSON {

std::string JSONNumber::toJSON() const {
  std::stringstream ss;
  ss << std::setprecision(16) << value_;
  return ss.str();
}
} // JSON