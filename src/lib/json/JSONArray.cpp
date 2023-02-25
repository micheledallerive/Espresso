//
// Created by michele on 25.02.23.
//

#include "JSONArray.h"
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

} // JSON