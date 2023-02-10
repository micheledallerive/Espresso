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
  bool set = false;
  virtual void valueChanged() {
    set = true;
  }
 public:
  typedef T value_type;
  PrimitiveWrapper() = default;
  virtual ~PrimitiveWrapper() = default;

  explicit PrimitiveWrapper(T val) : value(val) {
    valueChanged();
  }
  explicit PrimitiveWrapper(const T &val) : value(val) {
    valueChanged();
  }
  PrimitiveWrapper(const PrimitiveWrapper &wrapper)
      : value(wrapper.value) {
    valueChanged();
  }
  PrimitiveWrapper(PrimitiveWrapper &&wrapper) noexcept
      : value(wrapper.value) {
    valueChanged();
  }

  virtual PrimitiveWrapper<T> &operator=(const T &val) {
    this->value = val;
    valueChanged();
    return *this;
  }
  PrimitiveWrapper<T> &operator=(const PrimitiveWrapper<T> &other) {
    this->value = other.value;
    valueChanged();
    return *this;
  }
  PrimitiveWrapper<T> &operator=(PrimitiveWrapper<T> &&other) noexcept {
    this->value = std::move(other.value);
    valueChanged();
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
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator-=(const PrimitiveWrapper<T> &other) {
    this->value -= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator*=(const PrimitiveWrapper<T> &other) {
    this->value *= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator/=(const PrimitiveWrapper<T> &other) {
    this->value /= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator%=(const PrimitiveWrapper<T> &other) {
    this->value %= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator^=(const PrimitiveWrapper<T> &other) {
    this->value ^= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator&=(const PrimitiveWrapper<T> &other) {
    this->value &= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator|=(const PrimitiveWrapper<T> &other) {
    this->value |= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator<<=(const PrimitiveWrapper<T> &other) {
    this->value <<= other.value;
    valueChanged();
    return *this;
  }

  PrimitiveWrapper<T> &operator>>=(const PrimitiveWrapper<T> &other) {
    this->value >>= other.value;
    valueChanged();
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

  virtual explicit operator T() const { return this->value; }
  virtual operator T &() { return this->value; }
  virtual operator const T &() const { return this->value; }

  T get() const { return this->value; }

  // stream
  friend std::ostream &operator<<(std::ostream &os,
                                  const PrimitiveWrapper<T> &wrapper) {
    os << wrapper.value;
    return os;
  }

  friend std::istream &operator>>(std::istream &is,
                                  PrimitiveWrapper<T> &wrapper) {
    is >> wrapper.value;
    wrapper.valueChanged();
    return is;
  }
};

#include "PrimitiveWrapper.h"

} // ORM

#endif //ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_
