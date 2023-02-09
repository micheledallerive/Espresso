//
// Created by michele on 07.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
#define ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_

#include <string>
#include <unordered_map>
#include <any>
#include <mutex>
#include <shared_mutex>
#include <utility>
#include <orm/model/ModelData.h>
#include "orm/sql/SQLTypes.h"

namespace Espresso::ORM {

using std::string;
using std::unordered_map;

class ModelManager {
 public:
  static ModelManager &getInstance();

  template<class T, class... Args>
  void registerModel(const string &tableName, Args... args);

  template<class T>
  ModelData &getModel();

  void setAutomaticMigrations(bool);

  void migrateModels();

 protected:
  template<class T, class A, class... Args>
  void registerFields(A, Args ... args);

  void migrateModel(const std::string &typeInfo);

 private:
  ModelManager() = default;

  template<class T>
  void registerFields() {}

  template<class Class, class Type>
  ModelDataField setDataFieldTypes(Type Class::* field,
                                   ModelDataField &modelField);

  mutable std::shared_mutex mutex_;
  unordered_map<string, ModelData>
      models; // the type name of the model and the data
  bool autoMigrate{true};

 public:
  ModelManager(ModelManager const &) = delete;
  void operator=(ModelManager const &) = delete;
};

} // ORM

#include "ModelManager.cpp"
#include "ModelData.h"

#endif //ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
