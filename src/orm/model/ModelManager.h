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
#include "orm/sql/SQLTypes.h"

namespace Espresso::ORM {

using std::string;
using std::unordered_map;

struct ModelDataField {
  std::any field;
  std::string ctype;
  bool primaryKey{false};
};

struct ModelData {
  string tableName; // the name of the table for the model
  unordered_map<string, ModelDataField> fields; // field name and data
  bool migrated{false}; // whether the model was migrated
};

class ModelManager {
 public:
  static ModelManager &getInstance();

  template<class T, class... Args>
  void registerModel(const string &tableName, Args... args);

  template<class T>
  ModelData &getModel();

  void setAutomaticMigrations(bool);

 protected:
  template<class T, class A, class... Args>
  void registerFields(A, Args ... args);

  void migrateModel(const std::string &typeInfo);

 private:
  ModelManager() = default;

  template<class T>
  void registerFields() {}

  template<class Class, class Type> // Type Class::*field
  ModelDataField fieldToDataField(const Type Class::* field);

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

#endif //ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
