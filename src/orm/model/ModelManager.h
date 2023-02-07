//
// Created by michele on 07.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
#define ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_

#include <string>
#include <unordered_map>
#include "orm/sql/SQLTypes.h"

namespace Espresso::ORM {

using std::string;
using std::unordered_map;

struct ModelField {
  void *field;
  SQLType type;
};

struct ModelData {
  string tableName; // the name of the table for the model
  unordered_map<string, ModelField> fields;
};

class ModelManager {
 public:
  ModelManager() = delete;
  ~ModelManager() = delete;

  template<class T, class... Args>
  static void registerModel(const string &tableName, Args... args);

 protected:
  template<class T, class... Args>
  static void registerFields(T field, Args ... args);

 public:
  static void registerFields() {}
  static unordered_map<string, ModelData>
      models; // the type name of the model and the data
};

unordered_map<string, ModelData> ModelManager::models;

} // ORM

#include "ModelManager.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
