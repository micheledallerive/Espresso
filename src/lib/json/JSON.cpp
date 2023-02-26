//
// Created by michele on 25.02.23.
//
#include "JSON.h"
#include "JSONNumber.h"
#include "JSONBoolean.h"
#include "JSONLiteral.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "expections.h"

#include <sstream>
#include <stack>

namespace Espresso::JSON {

JSON *JSON::parse(const std::string &json, bool removeSpaces) {
  if (removeSpaces) {
    std::stringstream ss;
    bool inWord;
    for (const char &c : json) {
      if (c == '\"') {
        inWord = !inWord;
      } else if (!inWord && (isspace(c) || c == '\r' || c == '\n')) {
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
std::string JSON::nextToken(std::string::iterator start,
                            std::string::iterator end) {
  std::stack<char> parenthesis;
  bool everOpened = false;
  std::stringstream result;
  if (*start == '"') {
    do {
      result << *start;
      start++;
    } while (start != end && *start != '"');
    if (start == end)
      throw JSONParseException("Unbalanced parenthesis");
    result << *start;
    return result.str();
  }
  for (auto it = start; it != end; it++) {
    if (*it == '{' || *it == '[') {
      parenthesis.push(*it);
      everOpened = true;
    } else if (*it == '}' || *it == ']') {
      if (parenthesis.empty()) {
        if (everOpened)
          throw JSONParseException("Unbalanced parenthesis");
        else
          return result.str();
      }
      if (parenthesis.top() == '{' && *it == '}') {
        parenthesis.pop();
      } else if (parenthesis.top() == '[' && *it == ']') {
        parenthesis.pop();
      } else {
        throw JSONParseException("Unbalanced parenthesis");
      }
    } else if (*it == ',' && parenthesis.empty()) {
      return result.str();
    }
    result << *it;
  }
  return result.str();
}

}
