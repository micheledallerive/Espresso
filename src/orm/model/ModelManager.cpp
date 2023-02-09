//
// Created by michele on 07.02.23.
//

#include <mutex>
#include <set>
#include <vector>
#include <iostream>

#include <orm/database/DatabaseManager.h>
#include <orm/sql/SQLGenerator.h>
#include <orm/model/fields/PrimaryKey.h>
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
  using valType = Value;
};

template<class T, class A, class... Args>
void ModelManager::registerFields(A arg, Args ... args) {
  {
    std::unique_lock lock(this->mutex_);
    if (models.find(typeid(T).name()) == models.end()) {
      throw std::runtime_error("Model not registered");
    }

    ModelDataField modelField = fieldToDataField(arg.second);
    if (modelField.primaryKey) {
      if (!models[typeid(T).name()].primaryKey.empty()) {
        std::string msg =
            "Multiple primary keys: cannot set '" + string(arg.first)
                + "' as primary key";
        throw model_error(msg);
      }
      models[typeid(T).name()].primaryKey = arg.first;
    }

    models[typeid(T).name()].fields.emplace(arg.first, modelField);
  }
  registerFields < T >(args...);
}

template<class Class, class Type>
// Type Class::*field
ModelDataField ModelManager::fieldToDataField(Type Class::* field) {
  string fieldType =
      typeid(typename pointer_value<decltype(field)>::valType::value_type).name();
  ModelDataField modelField;
  // every field must be converted to a ModelField in order to be recasted back later
  // primary keys foreign keys etc are stored in the ModelDataField, so can be
  // upcasted to a ModelField
  modelField.field =
      reinterpret_cast<ModelField<typename Type::value_type> Class::*>(field);
  modelField.primaryKey = isPrimaryKey(field);
  modelField.ctype = fieldType;
  return modelField;
}

void ModelManager::migrateModels() {
  for (const auto &model : this->models) {
    if (model.second.migrated) {
      continue;
    }
    this->migrateModel(model.first);
  }
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

  vector<SQLColumnInfo> modelFieldsList = std::move(data.getColumns());
  if (dbFields.empty()) {
    // create the table
    string query = SQLGenerator::createTable(tableName, modelFieldsList, {});
    dbManager->execute(query);
    return;
  }

  set<SQLColumnInfo> modelFields = set<SQLColumnInfo>(modelFieldsList.begin(),
                                                      modelFieldsList.end());
  for (const auto &field : data.fields) {
    SQLColumnInfo info;
    info.name = field.first;
    info.type = getSQLType(field.second.ctype);
    modelFields.insert(info);
  }

  vector<SQLColumnInfo> toAdd;
  vector<SQLColumnInfo> toRemove;
  vector<SQLColumnInfo> toModify;

  std::set_difference(modelFields.begin(), modelFields.end(),
                      dbFields.begin(), dbFields.end(),
                      std::inserter(toAdd, toAdd.begin()));

  std::set_difference(dbFields.begin(), dbFields.end(),
                      modelFields.begin(), modelFields.end(),
                      std::inserter(toRemove, toRemove.begin()));

  vector<SQLColumnInfo> intersection;
  std::set_intersection(modelFields.begin(), modelFields.end(),
                        dbFields.begin(), dbFields.end(),
                        std::inserter(intersection, intersection.begin()),
                        [](const SQLColumnInfo &a, const SQLColumnInfo &b) {
                          return a.name == b.name && !a.equals(b);
                        });
  vector<SQLColumnInfo> nameIntersection;
  std::set_intersection(modelFields.begin(), modelFields.end(),
                        dbFields.begin(), dbFields.end(),
                        std::inserter(nameIntersection,
                                      nameIntersection.begin()));
  // add in toModify the elements that are not in the intersection
  for (const auto &field : modelFields) {
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
                                          nameIntersection,
                                          vector<SQLColumnInfo>(
                                              modelFields.begin(),
                                              modelFields.end()));
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