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
  bool dirty{false};
 public:
  typedef T value_type;
  PrimitiveWrapper() = default;
  explicit PrimitiveWrapper(T val) : value(val) {}
  explicit PrimitiveWrapper(const T &val) : value(val) {}
  PrimitiveWrapper(const PrimitiveWrapper &wrapper) : value(wrapper.value) {}
  PrimitiveWrapper(PrimitiveWrapper &&wrapper) noexcept: value(wrapper.value) {}

  PrimitiveWrapper<T> &operator=(const T &value) {
    value = value;
    return *this;
  }
  PrimitiveWrapper<T> &operator=(const PrimitiveWrapper<T> &other) {
    value = other.value;
    return *this;
  }
  PrimitiveWrapper<T> &operator=(PrimitiveWrapper<T> &&other) noexcept {
    value = std::move(other.value);
    return *this;
  }

  PrimitiveWrapper<T> operator+(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value + other.value);
  }

  PrimitiveWrapper<T> operator-(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value - other.value);
  }

  PrimitiveWrapper<T> operator*(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value * other.value);
  }

  PrimitiveWrapper<T> operator/(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value / other.value);
  }

  PrimitiveWrapper<T> operator%(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value % other.value);
  }

  PrimitiveWrapper<T> operator^(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value ^ other.value);
  }

  PrimitiveWrapper<T> operator&(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value & other.value);
  }

  PrimitiveWrapper<T> operator|(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value | other.value);
  }

  PrimitiveWrapper<T> operator~() const {
    return PrimitiveWrapper<T>(~value);
  }

  PrimitiveWrapper<T> operator<<(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value << other.value);
  }

  PrimitiveWrapper<T> operator>>(const PrimitiveWrapper<T> &other) const {
    return PrimitiveWrapper<T>(value >> other.value);
  }

  PrimitiveWrapper<T> &operator+=(const PrimitiveWrapper<T> &other) {
    value += other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator-=(const PrimitiveWrapper<T> &other) {
    value -= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator*=(const PrimitiveWrapper<T> &other) {
    value *= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator/=(const PrimitiveWrapper<T> &other) {
    value /= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator%=(const PrimitiveWrapper<T> &other) {
    value %= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator^=(const PrimitiveWrapper<T> &other) {
    value ^= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator&=(const PrimitiveWrapper<T> &other) {
    value &= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator|=(const PrimitiveWrapper<T> &other) {
    value |= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator<<=(const PrimitiveWrapper<T> &other) {
    value <<= other.value;
    return *this;
  }

  PrimitiveWrapper<T> &operator>>=(const PrimitiveWrapper<T> &other) {
    value >>= other.value;
    return *this;
  }

  bool operator<(const PrimitiveWrapper<T> &other) const {
    return value < other.value;
  }

  bool operator>(const PrimitiveWrapper<T> &other) const {
    return value > other.value;
  }

  bool operator<=(const PrimitiveWrapper<T> &other) const {
    return value <= other.value;
  }

  bool operator>=(const PrimitiveWrapper<T> &other) const {
    return value >= other.value;
  }

  bool operator==(const PrimitiveWrapper<T> &other) const {
    return value == other.value;
  }

  bool operator!=(const PrimitiveWrapper<T> &other) const {
    return value != other.value;
  }

  operator T() const { return value; }

  T get() const { return value; }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_
