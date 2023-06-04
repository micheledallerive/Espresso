//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONPRIMITIVE_H_
#define ESPRESSO_SRC_LIB_JSON_JSONPRIMITIVE_H_

#include <string>
#include "JSONEntity.h"

namespace Espresso::JSON {

template<typename T>
class JSONPrimitive : public JSONEntity {
 public:
  JSONPrimitive() = default;
  explicit JSONPrimitive(T value)
      : value_(value) {
  }
  ~JSONPrimitive() = default;
  JSONPrimitive &operator=(const JSONPrimitive &other) = default;
  JSONPrimitive &operator=(const T &other) {
    value_ = other;
    return *this;
  }

  T value() const {
    return value_;
  }
  operator T() const {
    return value_;
  }
 protected:
  T value_;
};

} // JSON

#include "lib/json/JSONPrimitive.cpp"

#endif //ESPRESSO_SRC_LIB_JSON_JSONPRIMITIVE_H_
