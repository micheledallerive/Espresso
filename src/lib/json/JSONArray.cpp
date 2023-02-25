//
// Created by michele on 25.02.23.
//

#include "JSONArray.h"
#include "expections.h"
#include "json.h"
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
JSONBase *JSONArray::fromJSON(const std::string &json) {
  if (json[0] != '[' || json[json.size() - 1] != ']')
    throw JSONParseException("Invalid JSON array");
  auto *array = new JSONArray();
  std::stringstream ss(json.substr(1, json.size() - 2));
  std::string token;
  while (std::getline(ss, token, ',')) {
    array->push_back(parse(token));
  }
  return array;
}

} // JSON