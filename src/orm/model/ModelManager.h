//
// Created by michele on 07.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
#define ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_

#include <string>
#include <unordered_map>
#include <any>
#include "orm/sql/SQLTypes.h"

namespace Espresso::ORM {

using std::string;
using std::unordered_map;

struct ModelField {
  std::any field;
  SQLType type;
  std::string ctype;
};

struct ModelData {
  string tableName; // the name of the table for the model
  unordered_map<string, ModelField> fields; // field name and data
};

class ModelManager {
 public:
  ModelManager() = delete;
  ~ModelManager() = delete;

  template<class T, class... Args>
  static void registerModel(const string &tableName, Args... args);

 protected:
  template<class T, class A, class... Args>
  static void registerFields(A, Args ... args);

 public:
  template<class T>
  static void registerFields() {}
  static unordered_map<string, ModelData>
      models; // the type name of the model and the data
};

unordered_map<string, ModelData> ModelManager::models;

} // ORM

#include "ModelManager.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_MODELMANAGER_H_
