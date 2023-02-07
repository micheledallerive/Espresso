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
  // T will be a pair<string, T> where T is the type of the field
  ModelField modelField = {&field.second, SQLType::TEXT};
  models[typeid(T).name()].fields.emplace(field.first, modelField);
  registerFields(args...);
}

} // ORM