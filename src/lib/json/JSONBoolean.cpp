//
// Created by michele on 24.02.23.
//

#include "lib/json/JSONBoolean.h"
#include "lib/json/expections.h"

namespace Espresso::JSON {

std::string JSONBoolean::toJSON() const {
  return value_ ? "true" : "false";
}
std::shared_ptr<JSONBoolean> JSONBoolean::fromJSON(const std::string &json) {
  if (json == "true") {
    return std::make_shared<JSONBoolean>(true);
  } else if (json == "false") {
    return std::make_shared<JSONBoolean>(false);
  }
  throw JSONParseException("Invalid JSON boolean");
}

}