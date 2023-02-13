//
// Created by michele on 07.02.23.
//

#include <set>
#include <vector>
#include <iostream>

#include <orm/database/DatabaseManager.h>
#include <orm/sql/SQLGenerator.h>
#include <orm/model/fields/PrimaryKey.h>
#include <orm/model/fields/ForeignKey.h>
#include "ModelManager.h"
#include "../exceptions.h"
#include "ModelData.h"

namespace Espresso::ORM {

ModelManager &ModelManager::getInstance() {
  static ModelManager m;
  return m;
}

template<class T>
ModelData &ModelManager::getModel() {
  auto it = this->models.find(typeid(T).name());
  if (it == this->models.end()) {
    throw model_error("Model not registered");
  }
  return it->second;
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
  if (models[typeid(T).name()].primaryKey.empty()) {
    throw model_error("No primary key set");
  }
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

    const Field &fieldParamData = arg.first;
    fieldParamData.validate();

    ModelDataField modelField(fieldParamData);

    modelField.primaryKey = fieldParamData.primaryKey
        || isPrimaryKey(arg.second);

    if (isForeignKey(arg.second)) {
      auto &foreign_key_field =
          dynamic_cast<ForeignKeyField &>(arg.first);
      std::string foreignObjectType =
          typeid(typename pointer_value<decltype(arg.second)>::valType::value_type).name();
      modelField.foreignKey = ForeignKeyData{
          .table=models[foreignObjectType].tableName,
          .tablePrimaryKey=models[foreignObjectType].primaryKey
      };
      modelField.ctype = typeid(std::string).name();
      modelField.field =
          reinterpret_cast<ModelField<std::string> T::*>(arg.second);

      std::string relatedName = foreign_key_field.relatedName;
      models[foreignObjectType].relatedModels.emplace(
          // to be changed, automatically the name of the table
          relatedName,
          std::make_pair(typeid(T).name(),
                         fieldParamData.name));
    } else {
      setDataFieldTypes(arg.second, modelField);
    }

    const std::string fieldName = fieldParamData.name;
    if (modelField.primaryKey) {
      if (!getModel<T>().primaryKey.empty()) {
        std::string msg =
            "Multiple primary keys: cannot set '" + string(fieldName)
                + "' as primary key";
        throw model_error(msg);
      }
      getModel<T>().primaryKey = fieldName;
    }

    getModel<T>().fields.emplace(fieldName, modelField);
  }
  registerFields < T >(args...);
}

template<class Class, class Type>
void ModelManager::setDataFieldTypes(Type Class::* field,
                                     ModelDataField &modelField) {
  string fieldType =
      typeid(typename pointer_value<decltype(field)>::valType::value_type).name();

  // every field must be converted to a ModelField in order to be recasted back later
  // primary keys foreign keys etc. are stored in the ModelDataField, so can be
  // upcasted to a ModelField
  modelField.field =
      reinterpret_cast<ModelField<typename Type::value_type> Class::*>(field);
  modelField.ctype = fieldType;
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

  vector<SQLColumnInfo> modelFieldsList = data.getColumns();
  if (dbFields.empty()) {
    // create the table
    string query = SQLGenerator::createTable(tableName, modelFieldsList);
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
//                                          toAdd,
//                                          toRemove,
//                                          toModify,
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