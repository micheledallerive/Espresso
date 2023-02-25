//
// Created by michele on 25.02.23.
//

#include "json.h"
#include "JSONLiteral.h"
#include "JSONNumber.h"
#include "JSONBoolean.h"
#include "JSONArray.h"

namespace Espresso::JSON {

JSONBase *parse(const std::string &json) {
  if (json[0] == '"') {
    return JSONLiteral::fromJSON(json);
  } else if (json[0] == 't' || json[0] == 'f') {
    return JSONBoolean::fromJSON(json);
  } else if (json[0] >= '0' && json[0] <= '9') {
    return JSONNumber::fromJSON(json);
  } else if (json[0] == '[') {
    return JSONArray::fromJSON(json);
  }
  return new JSONLiteral("");
}

} // JSON