//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONNUMBER_H_
#define ESPRESSO_SRC_LIB_JSON_JSONNUMBER_H_

#include <string>
#include "JSONPrimitive.h"

namespace Espresso::JSON {

class JSONNumber : public JSONPrimitive<double> {
 public:
  JSONNumber() : JSONPrimitive(0.0) {
    type_ = JSONType::JSONNumber;
  }
  explicit JSONNumber(double value) : JSONPrimitive(value) {
    type_ = JSONType::JSONNumber;
  }
  [[nodiscard]] std::string toJSON() const override;
  static JSONNumber *fromJSON(const std::string &json);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONNUMBER_H_
