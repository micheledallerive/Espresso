//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_
#define ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_

#include "JSONBase.h"
#include <unordered_map>

namespace Espresso::JSON {

class JSONObject : public JSONBase,
                   public std::unordered_map<std::string, JSONBase *> {
 public:
  JSONObject() : JSONBase(), std::unordered_map<std::string, JSONBase *>() {
    this->type_ = JSONType::JSONObject;
  }
  ~JSONObject() override;
  JSONObject &operator=(const JSONObject &other) = default;

  std::string toJSON() const override;
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_
