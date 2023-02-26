//
// Created by michele on 24.02.23.
//

#include "JSONLiteral.h"
#include "expections.h"

namespace Espresso::JSON {

std::string JSONLiteral::toJSON() const {
  return '"' + value_ + '"';
}
JSONLiteral *JSONLiteral::fromJSON(const std::string &json) {
  if (json[0] != '"' || json[json.size() - 1] != '"')
    throw JSONParseException("Invalid JSON literal: " + json);
  return new JSONLiteral(json.substr(1, json.size() - 2));
}

} // JSON