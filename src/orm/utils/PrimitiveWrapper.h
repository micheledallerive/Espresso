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
  virtual ~PrimitiveWrapper() = default;
  
  explicit PrimitiveWrapper(T val) : value(val), dirty(true) {}
  explicit PrimitiveWrapper(const T &val) : value(val), dirty(true) {}
  PrimitiveWrapper(const PrimitiveWrapper &wrapper)
      : value(wrapper.value), dirty(true) {}
  PrimitiveWrapper(PrimitiveWrapper &&wrapper) noexcept
      : value(wrapper.value), dirty(true) {}

  PrimitiveWrapper<T> &operator=(const T &val) {
    this->value = val;
    dirty = true;
    return *this;
  }
  PrimitiveWrapper<T> &operator=(const PrimitiveWrapper<T> &other) {
    this->value = other.value;
    dirty = true;
    return *this;
  }
  PrimitiveWrapper<T> &operator=(PrimitiveWrapper<T> &&other) noexcept {
    this->value = std::move(other.value);
    dirty = true;
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
    this->value += other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator-=(const PrimitiveWrapper<T> &other) {
    this->value -= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator*=(const PrimitiveWrapper<T> &other) {
    this->value *= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator/=(const PrimitiveWrapper<T> &other) {
    this->value /= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator%=(const PrimitiveWrapper<T> &other) {
    this->value %= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator^=(const PrimitiveWrapper<T> &other) {
    this->value ^= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator&=(const PrimitiveWrapper<T> &other) {
    this->value &= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator|=(const PrimitiveWrapper<T> &other) {
    this->value |= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator<<=(const PrimitiveWrapper<T> &other) {
    this->value <<= other.value;
    dirty = true;
    return *this;
  }

  PrimitiveWrapper<T> &operator>>=(const PrimitiveWrapper<T> &other) {
    this->value >>= other.value;
    dirty = true;
    return *this;
  }

  bool operator<(const PrimitiveWrapper<T> &other) const {
    return this->value < other.value;
  }

  bool operator>(const PrimitiveWrapper<T> &other) const {
    return this->value > other.value;
  }

  bool operator<=(const PrimitiveWrapper<T> &other) const {
    return this->value <= other.value;
  }

  bool operator>=(const PrimitiveWrapper<T> &other) const {
    return this->value >= other.value;
  }

  bool operator==(const PrimitiveWrapper<T> &other) const {
    return this->value == other.value;
  }

  bool operator!=(const PrimitiveWrapper<T> &other) const {
    return this->value != other.value;
  }

  explicit operator T() const { return this->value; }
  operator T &() { return this->value; }
  operator const T &() const { return this->value; }

  T get() const { return this->value; }
};

#include "PrimitiveWrapper.h"

} // ORM

#endif //ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_
