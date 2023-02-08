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

struct ModelField {
  std::any field;
  std::string ctype;
};

struct ModelData {
  string tableName; // the name of the table for the model
  unordered_map<string, ModelField> fields; // field name and data
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

 private:
  ModelManager() = default;

  template<class T>
  void registerFields() {}

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
