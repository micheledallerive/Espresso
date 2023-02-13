//
// Created by michele on 08.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODEL_H_
#define ESPRESSO_SRC_ORM_MODEL_MODEL_H_

#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <string>
#include <orm/model/fields/ForeignKey.h>
#include <orm/model/query/filter/FilterOperation.h>
#include <orm/model/query/QueryBuilder.h>

namespace Espresso::ORM {

template<class T>
class Model {
 public:
  Model() = default;
  virtual ~Model() = default;
  Model &operator=(const Model &other) = default;

  void save(bool checkDirty = true);
  bool remove();

  static Query::QueryBuilder<T> all();

  // operator ==
  bool operator==(const Model &other) const;
 protected:
  bool wasSaved{false};
 private:
  template<class G>
  friend
  class ForeignKey;
  template<class G>
  friend
  class Query::QueryBuilder;

  static ModelDataField &getFieldData(const std::string &fieldName);
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
