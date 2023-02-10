//
// Created by michele on 09.02.23.
//

#include "ForeignKey.h"

namespace Espresso::ORM {

template<class T>
const std::optional<T> &ForeignKey<T>::operator*() {
  if (!obj.has_value()) {
    if (!keyWasSet) {
      return std::nullopt;
    }
    const std::string
        &objPKName = ModelManager::getInstance().getModel<T>().primaryKey;
    obj = T::get({{objPKName, value}});
  }
  return obj.value();
}

template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(T &foreignObj) {
  obj = foreignObj;
  value = ModelManager::getInstance().getModel<T>().primaryKey;
  keyWasSet = true;
  return *this;
}
template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(const T &foreignObj) {
  ForeignKey<T>::operator=(const_cast<T &>(foreignObj));
  return *this;
}
template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(T *foreignObj) {
  if (foreignObj == nullptr) {
    obj = std::nullopt;
    value = "";
    keyWasSet = false;
  } else {
    ForeignKey<T>::operator=(*foreignObj);
  }
  return *this;
}

} // ORM