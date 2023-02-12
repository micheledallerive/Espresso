//
// Created by michele on 08.02.23.
//

#include "PrimitiveWrapper.h"

namespace Espresso::ORM {
template<typename T>
PrimitiveWrapper<T>::PrimitiveWrapper(T val) : value(val) {
  this->valueChanged();
}
template<typename T>
PrimitiveWrapper<T>::PrimitiveWrapper(const PrimitiveWrapper &wrapper)
    : value(wrapper.value) {
  this->valueChanged();
}
template<typename T>
PrimitiveWrapper<T>::PrimitiveWrapper(PrimitiveWrapper &&wrapper) noexcept
    : value(wrapper.value) {
  this->valueChanged();
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator=(const PrimitiveWrapper<T> &other) {
  this->value = other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator=(const T &val) {
  this->value = val;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator=(PrimitiveWrapper<T> &&other) noexcept {
  this->value = std::move(other.value);
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator+(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value + other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator-(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value - other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator*(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value * other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator/(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value / other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator%(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value % other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator^(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value ^ other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator&(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value & other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator~() const {
  return PrimitiveWrapper<T>(~value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator|(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value | other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator<<(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value << other.value);
}
template<typename T>
PrimitiveWrapper<T> PrimitiveWrapper<T>::operator>>(const PrimitiveWrapper<T> &other) const {
  return PrimitiveWrapper<T>(value >> other.value);
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator+=(const PrimitiveWrapper<T> &other) {
  this->value += other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator-=(const PrimitiveWrapper<T> &other) {
  this->value -= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator*=(const PrimitiveWrapper<T> &other) {
  this->value *= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator/=(const PrimitiveWrapper<T> &other) {
  this->value /= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator%=(const PrimitiveWrapper<T> &other) {
  this->value %= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator^=(const PrimitiveWrapper<T> &other) {
  this->value ^= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator&=(const PrimitiveWrapper<T> &other) {
  this->value &= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator|=(const PrimitiveWrapper<T> &other) {
  this->value |= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator<<=(const PrimitiveWrapper<T> &other) {
  this->value <<= other.value;
  valueChanged();
  return *this;
}
template<typename T>
PrimitiveWrapper<T> &PrimitiveWrapper<T>::operator>>=(const PrimitiveWrapper<T> &other) {
  this->value >>= other.value;
  valueChanged();
  return *this;
}
template<typename T>
bool PrimitiveWrapper<T>::operator<(const PrimitiveWrapper<T> &other) const {
  return this->value < other.value;
}
template<typename T>
bool PrimitiveWrapper<T>::operator>(const PrimitiveWrapper<T> &other) const {
  return this->value > other.value;
}
template<typename T>
bool PrimitiveWrapper<T>::operator<=(const PrimitiveWrapper<T> &other) const {
  return this->value <= other.value;
}
template<typename T>
bool PrimitiveWrapper<T>::operator>=(const PrimitiveWrapper<T> &other) const {
  return this->value >= other.value;
}
template<typename T>
bool PrimitiveWrapper<T>::operator==(const PrimitiveWrapper<T> &other) const {
  return this->value == other.value;
}
template<typename T>
bool PrimitiveWrapper<T>::operator!=(const PrimitiveWrapper<T> &other) const {
  return this->value != other.value;
}
template<typename T>
PrimitiveWrapper<T>::operator const T &() const { return this->value; }
template<typename T>
PrimitiveWrapper<T>::operator T() const { return this->value; }
template<typename T>
PrimitiveWrapper<T>::operator T &() { return this->value; }
template<typename T>
T PrimitiveWrapper<T>::get() const { return this->value; }

} // ORM