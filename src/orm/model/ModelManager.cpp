//
// Created by michele on 07.02.23.
//

#include "ModelManager.h"

namespace Espresso::ORM {

template<class T, class... Args>
void ModelManager::registerModel(const string &tableName, Args... args) {
  ModelData data;
  data.tableName = tableName;
  models.emplace(typeid(T).name(), data);
  registerFields(args...);
}

template<class T, class... Args>
void ModelManager::registerFields(T field, Args ... args) {
  ModelField modelField = {field, SQLType::TEXT};
  models[typeid(T).name()].fields.emplace(typeid(field).name(), modelField);
  registerFields(args...);
}

} // ORM