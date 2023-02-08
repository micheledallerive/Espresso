//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODEL_H_
#define ESPRESSO_SRC_ORM_MODEL_MODEL_H_

namespace Espresso::ORM {

template<class T>
class Model {
 public:
  Model() = default;
  ~Model() = default;

  static T get(int id);
};

} // ORM

#include "Model.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_MODEL_H_
