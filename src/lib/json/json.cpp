//
// Created by michele on 25.02.23.
//

#include "json.h"
#include "JSONLiteral.h"
#include "JSONNumber.h"
#include "JSONBoolean.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "expections.h"

namespace Espresso::JSON {

JSON *parse(const std::string &json) {
  if (json[0] == ' ' || json.back() == ' ') {
    auto start_it = json.begin();
    auto end_it = json.end();
    while (*start_it == ' ') {
      ++start_it;
    }
    while (*(end_it - 1) == ' ') {
      --end_it;
    }
    return parse(std::string(start_it, end_it));
  }
  if (json[0] == '"') {
    return JSONLiteral::fromJSON(json);
  } else if (json[0] == 't' || json[0] == 'f') {
    return JSONBoolean::fromJSON(json);
  } else if (json[0] >= '0' && json[0] <= '9') {
    return JSONNumber::fromJSON(json);
  } else if (json[0] == '[') {
    return JSONArray::fromJSON(json);
  } else if (json[0] == '{') {
    return JSONObject::fromJSON(json);
  } else if (json[0] == 'n' && json == "null") {
    return nullptr;
  }
  throw JSONParseException("Invalid JSON");
}

} // JSON