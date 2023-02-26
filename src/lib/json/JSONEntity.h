//
// Created by michele on 24.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONBASE_H_
#define ESPRESSO_SRC_LIB_JSON_JSONBASE_H_

#include <string>
#include <memory>

namespace Espresso::JSON {

enum class JSONType {
  JSONLiteral,
  JSONNumber,
  JSONBoolean,
  JSONObject,
  JSONArray,
};

class JSONEntity : public std::enable_shared_from_this<JSONEntity> {
 public:
  JSONEntity() = default;
  virtual ~JSONEntity() = default;
  JSONEntity &operator=(const JSONEntity &other) = default;

  virtual std::string toJSON() const = 0;

  [[nodiscard]] JSONType getType() const { return type_; }
  static std::shared_ptr<JSONEntity> parse(const std::string &json,
                                           bool removeSpaces = true);

  // how can I write a template that is child of JSONBase?
  template<class T>
  std::shared_ptr<T> as() {
    return std::dynamic_pointer_cast<T>(shared_from_this());
  }
 protected:
  JSONType type_;
  static std::string nextToken(std::string::iterator start,
                               std::string::iterator end);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSON_H_
