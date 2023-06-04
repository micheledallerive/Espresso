//
// Created by michele on 25.02.23.
//

#include "lib/json/json_array.h"
#include "lib/json/json_entity.h"
#include "lib/json/expections.h"
#include <sstream>

namespace Espresso::JSON {

std::string JSONArray::toJSON() const {
  if (this->empty())
    return "[ ]";
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
std::shared_ptr<JSONEntity> JSONArray::fromJSON(const std::string &json) {
  if (json[0] != '[' || json[json.size() - 1] != ']')
    throw JSONParseException("Invalid JSON array: " + json);
  auto array = std::make_shared<JSONArray>();
  std::string arrayContent = json.substr(1, json.size() - 2);
  auto it = arrayContent.begin();
  while (it < arrayContent.end()) {
    std::string token = JSONEntity::nextToken(it, arrayContent.end());
    it += token.size() + 1;
    array->push_back(JSONEntity::parse(token, false));
  }
  return array;
}

} // namespace Espresso::JSON