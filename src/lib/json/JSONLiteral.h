//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_
#define ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_

#include <utility>

#include "JSONBase.h"
namespace Espresso::JSON {

class JSONLiteral : public JSONBase {
 public:
  JSONLiteral() = default;
  explicit JSONLiteral(std::string value) : value_(std::move(value)) {}
  ~JSONLiteral() override = default;
  JSONLiteral &operator=(const JSONLiteral &other) = default;

  [[nodiscard]] std::string toJSON() const override;

 private:
  std::string value_;
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONLITERAL_H_
