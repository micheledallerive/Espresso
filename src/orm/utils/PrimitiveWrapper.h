//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_
#define ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_

#include <memory>

namespace Espresso::ORM {
template<typename T>
class PrimitiveWrapper {
 protected:
  T value;
 public:
  typedef T value_type;
  PrimitiveWrapper() = default;
  explicit PrimitiveWrapper(T val) : value(val) {}
  PrimitiveWrapper(const T &val) : value(val) {}
  PrimitiveWrapper(const PrimitiveWrapper &wrapper) : value(wrapper.value) {}
  PrimitiveWrapper(PrimitiveWrapper &&wrapper) noexcept: value(wrapper.value) {}
  virtual ~PrimitiveWrapper() = default;

  explicit operator T() const { return value; }

  PrimitiveWrapper &operator=(const PrimitiveWrapper &wrapper) {
    this->value = wrapper.value;
    return *this;
  }
  PrimitiveWrapper &operator=(const T &val) {
    this->value = val;
    return *this;
  }

  // write operator to write to ostream
  friend std::ostream &operator<<(std::ostream &os,
                                  const PrimitiveWrapper &wrapper) {
    os << wrapper.value;
    return os;
  }

};
} // ORM

#endif //ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_
