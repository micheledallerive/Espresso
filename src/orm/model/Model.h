//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODEL_H_
#define ESPRESSO_SRC_ORM_MODEL_MODEL_H_

#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <string>
#include <orm/model/fields/ForeignKey.h>

namespace Espresso::ORM {

template<class T>
class Model {
 public:
  Model() = default;
  virtual ~Model() = default;
  Model &operator=(const Model &other) = default;

  void save();
  bool remove();

  static T get(ConstraintMap);
  static std::shared_ptr<T> get_ptr(ConstraintMap);
 protected:
  bool wasSaved{false};
 private:
  template<class G>
  friend
  class ForeignKey;

  static void setFieldValue(T &instance,
                            ModelDataField &fieldData,
                            const std::string &value);
  static std::string getFieldValue(T &instance,
                                   ModelDataField &fieldData);
  static BaseModelField *getField(T &instance,
                                  ModelDataField &fieldData);
};

} // ORM

#include "Model.cpp"
#include "ModelData.h"

#endif //ESPRESSO_SRC_ORM_MODEL_MODEL_H_
