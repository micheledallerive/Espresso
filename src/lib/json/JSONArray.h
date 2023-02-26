//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONARRAY_H_
#define ESPRESSO_SRC_LIB_JSON_JSONARRAY_H_

#include "JSONEntity.h"
#include <vector>
#include <memory>

namespace Espresso::JSON {

class JSONArray
    : public JSONEntity, public std::vector<std::shared_ptr<JSONEntity> > {
 public:
  JSONArray() {
    this->type_ = JSONType::JSONArray;
  }
  ~JSONArray() override = default;
  JSONArray(const JSONArray &other) = default;
  JSONArray &operator=(const JSONArray &other) = default;

  [[nodiscard]] std::string toJSON() const override;
  static std::shared_ptr<JSONEntity> fromJSON(const std::string &json);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONARRAY_H_
