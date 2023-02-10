//
// Created by michele on 09.02.23.
//

#include "ForeignKey.h"

namespace Espresso::ORM {

template<class T>
const T &ForeignKey<T>::operator*() {
  if (!obj.has_value()) {
    const std::string
        &objPKName = ModelManager::getInstance().getModel<T>().primaryKey;
    obj = T::get({{objPKName, key.value()}});
  }
  return obj.value();
}

template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(T &value) {
  obj = value;
  key = value.pk;
  return *this;
}
template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(const T &value) {
  obj = value;
  key = value.pk;
  return *this;
}
} // ORM