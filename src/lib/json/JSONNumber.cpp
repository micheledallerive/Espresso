//
// Created by michele on 24.02.23.
//

#include "lib/json/JSONNumber.h"
#include "lib/json/expections.h"
#include <iomanip>

namespace Espresso::JSON {

std::string JSONNumber::toJSON() const {
  std::stringstream ss;
  ss << std::setprecision(16) << value_;
  return ss.str();
}
std::shared_ptr<JSONNumber> JSONNumber::fromJSON(const std::string &json) {
  try {
    return std::make_shared<JSONNumber>(std::stod(json));
  } catch (std::invalid_argument &e) {
    throw JSONParseException("Invalid JSON number");
  }
}
} // namespace Espresso::JSON