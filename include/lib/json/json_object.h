//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_
#define ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_

#include "json_entity.h"
#include <unordered_map>

namespace Espresso::JSON {

class JSONObject : public JSONEntity,
                   public std::unordered_map<std::string, std::shared_ptr<JSONEntity> > {
 public:
  JSONObject() : JSONEntity(), std::unordered_map<std::string, std::shared_ptr<JSONEntity> >() {
    this->type_ = JSONType::OBJECT;
  }
  ~JSONObject() override = default;
  JSONObject &operator=(const JSONObject &other) = default;

  std::string toJSON() const override;
  static std::shared_ptr<JSONEntity> fromJSON(const std::string &json);
};

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSONOBJECT_H_
