//
// Created by michele on 10.02.23.
//

#ifndef ESPRESSO_SRC_ORM_UTILS_SERIALIZABLE_H_
#define ESPRESSO_SRC_ORM_UTILS_SERIALIZABLE_H_

#include <string>

namespace Espresso::ORM {
class Serializable {
 public:
  virtual ~Serializable() = default;
  virtual std::string serialize() = 0;
  virtual void deserialize(const std::string &str) = 0;
};
}

#endif //ESPRESSO_SRC_ORM_UTILS_SERIALIZABLE_H_
