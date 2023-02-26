//
// Created by michele on 25.02.23.
//

#include "JSONObject.h"
#include "expections.h"
#include "JSONEntity.h"
#include <sstream>

namespace Espresso::JSON {

JSONObject::~JSONObject() {
  for (auto &pair : *this) {
    delete pair.second;
  }
}

[[nodiscard]] std::string JSONObject::toJSON() const {
  if (this->empty()) {
    return "{ }";
  }
  std::stringstream json;
  json << "{ ";
  for (auto it = this->begin(); it != this->end(); ++it) {
    json << "\"" << it->first << "\": " << it->second->toJSON();
    if (std::next(it) != this->end()) {
      json << ", ";
    }
  }
  json << " }";
  return json.str();
}

JSONEntity *JSONObject::fromJSON(const std::string &json) {
  if (json[0] != '{' || json[json.size() - 1] != '}') {
    throw JSONParseException("Invalid JSON object: " + json);
  }

  auto *object = new JSONObject();
  std::string objectContent = json.substr(1, json.size() - 2);
  auto it = objectContent.begin();
  while (it < objectContent.end()) {
    // read until the :
    std::string key;
    while (it < objectContent.end() && *it != ':') {
      key += *it;
      ++it;
    }
    if (it == objectContent.end()
        || key.empty()
        || key[0] != '"'
        || key[key.size() - 1] != '"') {
      throw JSONParseException("Invalid JSON object (key error): " + json);
    }
    key = key.substr(1, key.size() - 2);
    ++it;
    std::string value = JSONEntity::nextToken(it, objectContent.end());
    it += value.size() + 1;
    JSONEntity *parsed = JSONEntity::parse(value, false);
    (*object)[key] = parsed;
  }
  return object;
}

} // JSON