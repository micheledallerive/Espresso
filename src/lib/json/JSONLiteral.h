//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_
#define ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_

#include <utility>

#include "JSONBase.h"
#include "JSONPrimitive.h"
namespace Espresso::JSON {

class JSONLiteral : public JSONPrimitive<std::string> {
 public:
  JSONLiteral() {
    type_ = JSONType::JSONLiteral;
  }
  explicit JSONLiteral(std::string value)
      : JSONPrimitive<std::string>(std::move(value)) {
    type_ = JSONType::JSONLiteral;
  }
  ~JSONLiteral() override = default;
  JSONLiteral &operator=(const JSONLiteral &other) = default;

  [[nodiscard]] std::string toJSON() const override;
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_
