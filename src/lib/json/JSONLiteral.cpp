//
// Created by michele on 24.02.23.
//

#include "lib/json/JSONLiteral.h"
#include "lib/json/expections.h"

namespace Espresso::JSON {

std::string JSONLiteral::toJSON() const {
  return '"' + value_ + '"';
}
std::shared_ptr<JSONLiteral> JSONLiteral::fromJSON(const std::string &json) {
  if (json[0] != '"' || json[json.size() - 1] != '"')
    throw JSONParseException("Invalid JSON literal: " + json);
  return std::make_shared<JSONLiteral>(json.substr(1, json.size() - 2));
}

} // JSON