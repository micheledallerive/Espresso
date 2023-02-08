//
// Created by michele on 07.02.23.
//

#include <mutex>
#include <set>
#include <vector>
#include <iostream>

#include <orm/database/DatabaseManager.h>
#include <orm/sql/SQLGenerator.h>
#include "ModelManager.h"
#include "../exceptions.h"


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
    ModelDataField ModelDataField =
        {std::any(arg.second),
            typeid(typename pointer_value<decltype(arg.second)>::type).name()};
    models[typeid(T).name()].fields.emplace(arg.first, ModelDataField);
  }
  registerFields < T >(args...);
}

void ModelManager::migrateModel(const std::string &typeInfo) {
  using std::set, std::vector, std::string;
  const ModelData &data = this->models[typeInfo];
  if (data.migrated) {
    return;
  }

  const string &tableName = data.tableName;
  set<SQLColumnInfo> dbFields;
  dbManager->execute("PRAGMA table_info(" + tableName + ")",
                     [&dbFields](const unordered_map<string, string> &result) {
                       SQLColumnInfo info(result);
                       dbFields.insert(info);
                     });

  if (dbFields.empty()) {
    // create the table
    vector<string> fields;
    vector<string> types;
    for (const auto &field : data.fields) {
      fields.push_back(field.first);
      types.push_back(to_string(getSQLType(field.second.ctype)));
    }
    string query = SQLGenerator::createTable(tableName, fields, types, {});
    dbManager->execute(query);
    return;
  }

  set<SQLColumnInfo> ModelDataFields;
  for (const auto &field : data.fields) {
    SQLColumnInfo info;
    info.name = field.first;
    info.type = getSQLType(field.second.ctype);
    ModelDataFields.insert(info);
  }

  vector<SQLColumnInfo> toAdd;
  vector<SQLColumnInfo> toRemove;
  vector<SQLColumnInfo> toModify;

  std::set_difference(ModelDataFields.begin(), ModelDataFields.end(),
                      dbFields.begin(), dbFields.end(),
                      std::inserter(toAdd, toAdd.begin()));

  std::set_difference(dbFields.begin(), dbFields.end(),
                      ModelDataFields.begin(), ModelDataFields.end(),
                      std::inserter(toRemove, toRemove.begin()));

  vector<SQLColumnInfo> intersection;
  std::set_intersection(ModelDataFields.begin(), ModelDataFields.end(),
                        dbFields.begin(), dbFields.end(),
                        std::inserter(intersection, intersection.begin()));
  // add in toModify the elements that are not in the intersection
  for (const auto &field : ModelDataFields) {
    if (std::find(intersection.begin(), intersection.end(), field) ==
        intersection.end()) {
      toModify.push_back(field);
    }
  }

  if (toAdd.size() + toRemove.size() + toModify.size() > 0) {
    std::cout << "Migrating model " << tableName << std::endl;

    // lets assume the table exists
    string sql = SQLGenerator::alterTable(tableName,
                                          toAdd,
                                          toRemove,
                                          toModify,
                                          intersection,
                                          vector<SQLColumnInfo>(ModelDataFields.begin(),
                                                                ModelDataFields.end()));
    dbManager->execute(sql);
  }

  // update the model
  {
    std::unique_lock lock(this->mutex_);
    this->models[typeInfo].migrated = true;
  }

}

void ModelManager::setAutomaticMigrations(bool automatic) {
  this->autoMigrate = automatic;
}

} // ORM