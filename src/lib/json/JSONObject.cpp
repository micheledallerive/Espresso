//
// Created by michele on 25.02.23.
//

#include "JSONObject.h"
#include "expections.h"
#include "JSON.h"
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

JSON *JSONObject::fromJSON(const std::string &json) {
  if (json[0] != '{' || json[json.size() - 1] != '}') {
    throw JSONParseException("Invalid JSON object");
  }

  auto *object = new JSONObject();
  std::stringstream ss(json.substr(1, json.size() - 2));
  std::string key;
  std::string value;
  std::string keyValuePair;
  while (std::getline(ss, keyValuePair, ',')) {
    std::stringstream ss2(keyValuePair);
    std::getline(ss2, key, ':');
    std::getline(ss2, value);
    key = key.substr(1, key.size() - 2);
    object->insert({key, JSON::parse(value, false)});
  }
  return object;
}

} // JSON