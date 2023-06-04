//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_
#define ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_

#include <utility>

#include "json_entity.h"
#include "json_primitive.h"
namespace Espresso::JSON {

class JSONLiteral : public JSONPrimitive<std::string> {
 public:
  JSONLiteral() {
    type_ = JSONType::LITERAL;
  }
  explicit JSONLiteral(std::string value)
      : JSONPrimitive<std::string>(std::move(value)) {
    type_ = JSONType::LITERAL;
  }
  ~JSONLiteral() override = default;
  JSONLiteral &operator=(const JSONLiteral &other) = default;

  [[nodiscard]] std::string toJSON() const override;

  static std::shared_ptr<JSONLiteral> fromJSON(const std::string &json);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_
