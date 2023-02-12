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

  explicit PrimitiveWrapper(T val);
  PrimitiveWrapper(const PrimitiveWrapper &wrapper);
  PrimitiveWrapper(PrimitiveWrapper &&wrapper) noexcept;

  virtual PrimitiveWrapper<T> &operator=(const T &val);
  PrimitiveWrapper<T> &operator=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator=(PrimitiveWrapper<T> &&other) noexcept;

  PrimitiveWrapper<T> operator+(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator-(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator*(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator/(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator%(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator^(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator&(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator|(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator~() const;
  PrimitiveWrapper<T> operator<<(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator>>(const PrimitiveWrapper<T> &other) const;
  PrimitiveWrapper<T> operator<<(const T &other) const;
  PrimitiveWrapper<T> operator>>(const T &other) const;

  PrimitiveWrapper<T> &operator+=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator-=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator*=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator/=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator%=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator^=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator&=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator|=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator<<=(const PrimitiveWrapper<T> &other);
  PrimitiveWrapper<T> &operator>>=(const PrimitiveWrapper<T> &other);

  // increment and decrement
  PrimitiveWrapper<T> &operator++();
  const PrimitiveWrapper<T> operator++(T);
  PrimitiveWrapper<T> &operator--();
  const PrimitiveWrapper<T> operator--(T);

  bool operator<(const PrimitiveWrapper<T> &other) const;
  bool operator>(const PrimitiveWrapper<T> &other) const;
  bool operator<=(const PrimitiveWrapper<T> &other) const;
  bool operator>=(const PrimitiveWrapper<T> &other) const;
  bool operator==(const PrimitiveWrapper<T> &other) const;
  bool operator!=(const PrimitiveWrapper<T> &other) const;

  virtual operator T &();
  virtual operator const T &() const;
  T get() const;

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

} // ORM
#include "PrimitiveWrapper.cpp"

#endif //ESPRESSO_SRC_ORM_UTILS_PRIMITIVEWRAPPER_H_
