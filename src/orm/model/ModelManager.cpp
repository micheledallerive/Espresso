//
// Created by michele on 07.02.23.
//

#include "ModelManager.h"

namespace Espresso::ORM {

template<class T, class... Args>
void ModelManager::registerModel(const string &tableName, Args... args) {
  ModelData data;
  data.tableName = tableName;
  models[typeid(T).name()] = data;
  registerFields<T>(args...);
}

template<class T, class A, class... Args>
void ModelManager::registerFields(A arg, Args ... args) {
  if (models.find(typeid(T).name()) == models.end()) {
    throw std::runtime_error("Model not registered");
  }
  // create a void pointer to args.second
  ModelField modelField = {std::any(arg.second), SQLType::TEXT};
  models[typeid(T).name()].fields.emplace(arg.first, modelField);
  registerFields<T>(args...);
}

} // ORM