//
// Created by michele on 24.02.23.
//

#include "JSONBoolean.h"
#include "expections.h"

namespace Espresso::JSON {

std::string JSONBoolean::toJSON() const {
  return value_ ? "true" : "false";
}
JSONBoolean *JSONBoolean::fromJSON(const std::string &json) {
  if (json == "true") {
    return new JSONBoolean(true);
  } else if (json == "false") {
    return new JSONBoolean(false);
  }
  throw JSONParseException("Invalid JSON boolean");
}

}