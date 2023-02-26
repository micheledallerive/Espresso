//
// Created by michele on 25.02.23.
//

#include "JSONArray.h"
#include "expections.h"
#include "JSON.h"
#include <sstream>

namespace Espresso::JSON {

JSONArray::~JSONArray() {
  for (auto it = this->begin(); it != this->end(); ++it) {
    delete *it;
  }
}

std::string JSONArray::toJSON() const {
  if (this->empty()) return "[ ]";
  std::stringstream json;
  json << "[";
  for (auto it = this->begin(); it != this->end(); ++it) {
    if (*it) {
      json << (*it)->toJSON();
    } else {
      json << "null";
    }
    if (it + 1 != this->end()) {
      json << ", ";
    }
  }
  json << "]";
  return json.str();
}
JSON *JSONArray::fromJSON(const std::string &json) {
  if (json[0] != '[' || json[json.size() - 1] != ']')
    throw JSONParseException("Invalid JSON array: " + json);
  auto *array = new JSONArray();
  std::string arrayContent = json.substr(1, json.size() - 2);
  auto it = arrayContent.begin();
  while (it < arrayContent.end()) {
    std::string token = JSON::nextToken(it, arrayContent.end());
    it += token.size() + 1;
    array->push_back(JSON::parse(token, false));
  }
  return array;
}

} // JSON