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

class JSON {
 public:
  JSON() = default;
  virtual ~JSON() = default;
  JSON &operator=(const JSON &other) = default;

  virtual std::string toJSON() const = 0;

  [[nodiscard]] JSONType getType() const { return type_; }
  static JSON *parse(const std::string &json, bool removeSpaces = true);

  // how can I write a template that is child of JSONBase?
  template<class T>
  T *as();
 protected:
  JSONType type_;
};
template<class T>
T *JSON::as() {
  return dynamic_cast<T *>(this);
}

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSON_H_
