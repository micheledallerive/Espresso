//
// Created by michele on 25.02.23.
//
#include "JSON.h"
#include "JSONNumber.h"
#include "JSONBoolean.h"
#include "JSONLiteral.h"
#include "JSONArray.h"
#include "JSONObject.h"

#include <sstream>

namespace Espresso::JSON {

JSON *JSON::parse(const std::string &json, bool removeSpaces) {
  if (removeSpaces) {
    std::stringstream ss;
    bool inWord;
    for (const char &c : json) {
      if (c == '\"') {
        inWord = !inWord;
      } else if (!inWord && c == ' ') {
        continue;
      }
      ss << c;
    }
    return parse(ss.str(), false);
  }
  if (json[0] == '{') {
    return JSONObject::fromJSON(json);
  } else if (json[0] == '[') {
    return JSONArray::fromJSON(json);
  } else if (json[0] == '"') {
    return JSONLiteral::fromJSON(json);
  } else if (json == "true" || json == "false") {
    return JSONBoolean::fromJSON(json);
  } else {
    return JSONNumber::fromJSON(json);
  }
}

}
