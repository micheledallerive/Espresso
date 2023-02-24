//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONBASE_H_
#define ESPRESSO_SRC_LIB_JSON_JSONBASE_H_

#include <string>

namespace Espresso::JSON {

enum class JSONType {
  JSONLiteral,
  JSONNumber,
  JSONBoolean,
  JSONObject,
  JSONArray,
};

class JSONBase {
 public:
  JSONBase() = default;
  virtual ~JSONBase() = default;
  JSONBase &operator=(const JSONBase &other) = default;

  virtual std::string toJSON() const = 0;
 protected:
  JSONType type_;
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONBASE_H_
