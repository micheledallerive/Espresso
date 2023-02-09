//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODEL_H_
#define ESPRESSO_SRC_ORM_MODEL_MODEL_H_

#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <string>

namespace Espresso::ORM {

template<class T>
class Model {
 public:
  Model() = default;
  virtual ~Model() = default;
  void save();
  bool remove();

  static T get(ConstraintMap);
 protected:
  bool wasSaved{false};
 private:
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
