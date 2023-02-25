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

  [[nodiscard]] JSONType getType() const { return type_; }

  // how can I write a template that is child of JSONBase?
  template<class T>
  T *as();
 protected:
  JSONType type_;
};
template<class T>
T *JSONBase::as() {
  return dynamic_cast<T *>(this);
}

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONBASE_H_
