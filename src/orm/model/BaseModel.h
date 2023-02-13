//
// Created by michele on 13.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_BASEMODEL_H_
#define ESPRESSO_SRC_ORM_MODEL_BASEMODEL_H_

#include <string>

namespace Espresso::ORM {

class BaseModel {
 public:
  virtual ~BaseModel() = default;

  virtual void set(const std::string &, const std::string &) {};
  virtual std::string get(const std::string &) const { return ""; };
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_BASEMODEL_H_
