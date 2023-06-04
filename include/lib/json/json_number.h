//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONNUMBER_H_
#define ESPRESSO_SRC_LIB_JSON_JSONNUMBER_H_

#include <string>
#include "json_primitive.h"

namespace Espresso::JSON {

class JSONNumber : public JSONPrimitive<double> {
 public:
  JSONNumber() : JSONPrimitive(0.0) {
    type_ = JSONType::NUMBER;
  }
  explicit JSONNumber(double value) : JSONPrimitive(value) {
    type_ = JSONType::NUMBER;
  }
  [[nodiscard]] std::string toJSON() const override;
  static std::shared_ptr<JSONNumber> fromJSON(const std::string &json);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONNUMBER_H_
