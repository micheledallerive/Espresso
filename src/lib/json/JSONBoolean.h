//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONBOOLEAN_H_
#define ESPRESSO_SRC_LIB_JSON_JSONBOOLEAN_H_

#include "JSONPrimitive.h"

namespace Espresso::JSON {

class JSONBoolean : public JSONPrimitive<bool> {
 public:
  JSONBoolean() : JSONPrimitive(false) {
    type_ = JSONType::JSONBoolean;
  }
  explicit JSONBoolean(bool value) : JSONPrimitive(value) {
    type_ = JSONType::JSONBoolean;
  }
  std::string toJSON() const override;
  static std::shared_ptr<JSONBoolean> fromJSON(const std::string &json);
};

}

#endif //ESPRESSO_SRC_LIB_JSON_JSONBOOLEAN_H_
