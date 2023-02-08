//
// Created by michele on 07.02.23.
//

#include "ModelManager.h"
#include "../exceptions.h"
#include <mutex>

namespace Espresso::ORM {

ModelManager &ModelManager::getInstance() {
  static ModelManager m;
  return m;
}

template<class T>
ModelData &ModelManager::getModel() {
  std::shared_lock lock(this->mutex_);
  return this->models[typeid(T).name()];
}

template<class T, class... Args>
void ModelManager::registerModel(const string &tableName, Args... args) {
  {
    std::unique_lock lock(this->mutex_);
    ModelData data;
    data.tableName = tableName;
    models[typeid(T).name()] = data;
  }
  registerFields<T>(args...);
  if (this->autoMigrate) {
    this->migrateModel(typeid(T).name());
  }
}

template<typename T>
struct pointer_value;
template<typename Class, typename Value>
struct pointer_value<Value Class::*> {
  using type = Value;
};

template<class T, class A, class... Args>
void ModelManager::registerFields(A arg, Args ... args) {
  {
    std::unique_lock lock(this->mutex_);
    if (models.find(typeid(T).name()) == models.end()) {
      throw std::runtime_error("Model not registered");
    }
    // create a void pointer to args.second
    ModelField modelField =
        {std::any(arg.second),
            typeid(typename pointer_value<decltype(arg.second)>::type).name()};
    models[typeid(T).name()].fields.emplace(arg.first, modelField);
  }
  registerFields < T >(args...);
}

void ModelManager::migrateModel(const std::string &typeInfo) {
  const ModelData &data = this->models[typeInfo];
  if (data.migrated) {
    return;
  }

  const string &tableName = data.tableName;
  dbManager->execute("PRAGMA table_info(" + tableName + ")",
                     [](const unordered_map<string, string> &result) {
                       for (const auto p : result) {
                         std::cout << p.first << " " << p.second << std::endl;
                       }
                     });
}

void ModelManager::setAutomaticMigrations(bool automatic) {
  this->autoMigrate = automatic;
}

} // ORM