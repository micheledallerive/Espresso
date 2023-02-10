//
// Created by michele on 09.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_

namespace Espresso::ORM {

// create a class ForeignKey<T> that inherits from ModelField<T> but allows self-reference to the model
// and might be empty
template<class T>
class ForeignKey : public ModelField<string> {
 private:
  // the object the foreign key represents
  // the object will be computed only when accessed
  bool keyWasSet{false};
  std::optional<T *> obj{std::nullopt};

 public:
  using value_type = T;
  ForeignKey() = default;
  ~ForeignKey() override = default;
  ForeignKey(const ForeignKey<T> &);
  ForeignKey(ForeignKey<T> &&) noexcept;

  // used by the user to set the foreign "object"
  ForeignKey &operator=(T &);
  ForeignKey &operator=(const T &);
  ForeignKey &operator=(T *);

  ForeignKey &operator=(const ForeignKey<T> &);

  // used by the user to get the foreign "object"
  std::optional<T *> operator*();
};

// write functions to check if a field is a foreign key
template<typename T, typename U>
bool isForeignKey(const T U::*) {
  return false;
}

template<typename T, typename U>
bool isForeignKey(const ForeignKey<T> U::*) {
  return true;
}

} // ORM

#include "ForeignKey.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_
