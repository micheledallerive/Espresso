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
#include "BaseModel.h"

namespace Espresso::ORM {

template<class T>
class Model : public BaseModel{
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
  BaseModelField *getField(const std::string &field);
  void set(const std::string &field, const std::string &value) override;
  std::string get(const std::string &field) const override;

};

} // ORM

#include "Model.cpp"
#include "ModelData.h"

#endif //ESPRESSO_SRC_ORM_MODEL_MODEL_H_
