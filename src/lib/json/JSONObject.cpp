//
// Created by michele on 25.02.23.
//

#include "JSONObject.h"
#include <sstream>

namespace Espresso::JSON {

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
JSONObject::~JSONObject() {
  for (auto &pair : *this) {
    delete pair.second;
  }
}

} // JSON