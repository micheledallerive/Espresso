//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONARRAY_H_
#define ESPRESSO_SRC_LIB_JSON_JSONARRAY_H_

#include "JSONBase.h"
#include <vector>
#include <memory>

namespace Espresso::JSON {

class JSONArray
    : public JSONBase, public std::vector<JSONBase *> {
 public:
  JSONArray() = default;
  ~JSONArray() override;
  JSONArray(const JSONArray &other) = default;
  JSONArray &operator=(const JSONArray &other) = default;

  [[nodiscard]] std::string toJSON() const override;
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONARRAY_H_
