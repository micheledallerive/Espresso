//
// Created by michele on 24.02.23.
//

#include <iomanip>
#include "JSONNumber.h"
#include "expections.h"

namespace Espresso::JSON {

std::string JSONNumber::toJSON() const {
  std::stringstream ss;
  ss << std::setprecision(16) << value_;
  return ss.str();
}
JSONNumber *JSONNumber::fromJSON(const std::string &json) {
  try {
    return new JSONNumber(std::stod(json));
  } catch (std::invalid_argument &e) {
    throw JSONParseException("Invalid JSON number");
  }
}
} // JSON