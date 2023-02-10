//
// Created by michele on 09.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_

#include <orm/model/fields/ModelField.h>

namespace Espresso::ORM {

// create a class ForeignKey<T> that inherits from ModelField<T> but allows self-reference to the model
// and might be empty
template<class T>
class ForeignKey : public ModelField<string> {
 private:
  // the object the foreign key represents
  // the object will be computed only when accessed
  bool keyWasSet{false};
  std::optional<const T &> obj{std::nullopt};

 public:
  ForeignKey() = default;
  virtual ~ForeignKey() = default;

  // used by the user to set the foreign "object"
  ForeignKey &operator=(T &);
  ForeignKey &operator=(const T &);
  ForeignKey &operator=(T *);
  ForeignKey &operator=(ForeignKey<T> &) = delete;
  ForeignKey &operator=(const ForeignKey<T> &) = delete;

  // used by the user to get the foreign "object"
  const std::optional<T> &operator*();
};

} // ORM

#include "ForeignKey.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FOREIGNKEY_H_
