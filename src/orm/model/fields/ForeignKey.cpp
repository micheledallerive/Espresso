//
// Created by michele on 09.02.23.
//

#include "ForeignKey.h"

namespace Espresso::ORM {

template<class T>
ForeignKey<T>::ForeignKey(const ForeignKey<T> &foreignObj) : ModelField<string>(
    foreignObj) {
  obj = foreignObj.obj;
  keyWasSet = foreignObj.keyWasSet;
}

template<class T>
ForeignKey<T>::ForeignKey(ForeignKey<T> &&foreignObj) noexcept
    : ModelField<string>(std::move(foreignObj)) {
  obj = std::move(foreignObj.obj);
  keyWasSet = foreignObj.keyWasSet;
}

template<class T>
std::shared_ptr<T> ForeignKey<T>::operator*() {
  if (!obj.has_value()) {
    if (!keyWasSet && this->value.empty()) {
      return nullptr;
    }
    const std::string
        &objPKName = ModelManager::getInstance().getModel<T>().primaryKey;
    obj = T::get_ptr({{objPKName, value}});
  }
  return obj.value();
}

template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(T &foreignObj) {
  obj = std::make_shared<T>(foreignObj);
  std::string pk = ModelManager::getInstance().getModel<T>().primaryKey;
  ModelDataField
      &fieldData = ModelManager::getInstance().getModel<T>().fields[pk];
  value = foreignObj.getFieldValue(foreignObj, fieldData);
  keyWasSet = true;
  setDirty(true);
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
  setDirty(true);
  return *this;
}
template<class T>
ForeignKey<T> &ForeignKey<T>::operator=(const ForeignKey<T> &foreignObj) {
  obj = foreignObj.obj;
  value = foreignObj.value;
  keyWasSet = foreignObj.keyWasSet;
  return *this;
}

} // ORM