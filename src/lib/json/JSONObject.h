//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_
#define ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_

#include "JSON.h"
#include <unordered_map>

namespace Espresso::JSON {

class JSONObject : public JSON,
                   public std::unordered_map<std::string, JSON *> {
 public:
  JSONObject() : JSON(), std::unordered_map<std::string, JSON *>() {
    this->type_ = JSONType::JSONObject;
  }
  ~JSONObject() override;
  JSONObject &operator=(const JSONObject &other) = default;

  std::string toJSON() const override;
  static JSON *fromJSON(const std::string &json);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_
