//
// Created by michele on 08.02.23.
//

#include "Model.h"
#include "ModelData.h"
#include <orm/model/fields/BaseModelField.h>
#include <orm/model/query/filter/FilterOperation.h>
#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <orm/sql/SQLGenerator.h>
#include <orm/exceptions.h>

#include <iostream>

namespace Espresso::ORM {

template<class T>
Query::QueryBuilder<T> Model<T>::all() {
  return Query::QueryBuilder<T>();
}

template<class T>
std::shared_ptr<T> Model<T>::get_ptr(const std::optional<Query::FilterOperation> &constraints) {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  string query = SQLGenerator::select(data.tableName, {"*"}, constraints);
  std::shared_ptr<T> instance = std::make_shared<T>();
  bool found = false;
  const std::vector<std::string> &keys =
      constraints.has_value()
          ? constraints.value().getKeys()
          : std::vector<std::string>();
  for (const auto &key : keys) {
    if (!data.fields.contains(key)) {
      throw model_error("Field " + key + " does not exist");
    }
  }
  dbManager->execute(query,
                     [&data, &instance, &found](std::unordered_map<std::string,
                                                                   std::string> &result) {
                       if (!data.fields.empty()) found = true;
                       for (auto &ModelDataField : data.fields) {
                         setFieldValue(*instance,
                                       ModelDataField.second,
                                       result[ModelDataField.first]);
                         auto *field =
                             getField(*instance, ModelDataField.second);
                         field->dirty = false;
                       }
                     });
  if (!found) {
    throw object_not_found("Object not found");
  }
  instance->wasSaved = true;
  return instance;
}

template<class T>
T Model<T>::get(const std::optional<Query::FilterOperation> &constraints) {
  return *get_ptr(constraints);
}

template<class T>
bool Model<T>::remove() {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  T *instance = static_cast<T *>(this);
  if (!instance->wasSaved) {
    throw model_error("Cannot remove an unsaved object");
  }
  std::string
      primaryKeyField = ModelManager::getInstance().getModel<T>().primaryKey;
  std::string primaryKeyValue = getFieldValue(*instance,
                                              data.fields[primaryKeyField]);
  string query = SQLGenerator::remove(data.tableName,
                                      {{primaryKeyField, primaryKeyValue}});
  dbManager->execute(query);
  return true;
}

template<class T>
void Model<T>::save() {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  T *instance = static_cast<T *>(this);

  std::vector<string> fields;
  std::vector<string> values;
  for (auto &modelField : data.fields) {
    BaseModelField *baseField = getField(*instance, modelField.second);
    bool dirty = baseField->dirty;
    if (modelField.second.primaryKey) {
      if (wasSaved && dirty) {
        throw model_error("Cannot change primary key of an existing object");
      }
      if (!wasSaved && !dirty && !modelField.second.autoIncrement) {
        throw model_error("Non autoincrement primary key must be set");
      }
      if (!wasSaved && !dirty && modelField.second.autoIncrement) {
        continue;
      }
    }

    if (!dirty) continue;

    fields.push_back(modelField.first);
    values.push_back(getFieldValue(*instance, modelField.second));
  }

  if (!wasSaved) { // instance does not exist: insert
    string query = SQLGenerator::insert(data.tableName, fields, values);
    dbManager->execute(query,
                       [&instance, &data](std::unordered_map<std::string,
                                                             std::string> &result) {
                         if (result.contains("id") && !data.primaryKey.empty())
                           setFieldValue(*instance,
                                         data.fields[data.primaryKey],
                                         result["id"]);
                       });
    wasSaved = true;
  } else { // already exists: update it
    std::string
        primaryKeyField = ModelManager::getInstance().getModel<T>().primaryKey;
    std::string primaryKeyValue = getFieldValue(*instance,
                                                data.fields[primaryKeyField]);
    string query = SQLGenerator::update(data.tableName,
                                        fields,
                                        values,
                                        {{primaryKeyField, primaryKeyValue}});
    dbManager->execute(query);
  }
}

template<class T>
void Model<T>::setFieldValue(T &instance,
                             ModelDataField &fieldData,
                             const std::string &value) {
  if (fieldData.ctype == typeid(std::string).name()) {
    instance.*std::any_cast<ModelField<std::string> T::*>(fieldData.field) =
        value;
  } else if (fieldData.ctype == typeid(int).name()) {
    instance.*std::any_cast<ModelField<int> T::*>(fieldData.field) =
        std::stoi(value);
  } else if (fieldData.ctype == typeid(float).name()) {
    instance.*std::any_cast<ModelField<float> T::*>(fieldData.field) =
        std::stof(value);
  } else if (fieldData.ctype == typeid(double).name()) {
    instance.*std::any_cast<ModelField<double> T::*>(fieldData.field) =
        std::stod(value);
  } else if (fieldData.ctype == typeid(bool).name()) {
    instance.*std::any_cast<ModelField<bool> T::*>(fieldData.field) =
        std::stoi(value);
  } else if (fieldData.ctype == typeid(long long).name()) {
    instance.*std::any_cast<ModelField<long long> T::*>(fieldData.field) =
        std::stoll(value);
  } else if (fieldData.ctype == typeid(unsigned long long).name()) {
    instance
        .*std::any_cast<ModelField<unsigned long long> T::*>(fieldData.field) =
        std::stoull(value);
  } else if (fieldData.ctype == typeid(unsigned int).name()) {
    instance.*std::any_cast<ModelField<unsigned int> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(unsigned short).name()) {
    instance.*std::any_cast<ModelField<unsigned short> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(unsigned char).name()) {
    instance.*std::any_cast<ModelField<unsigned char> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(long).name()) {
    instance.*std::any_cast<ModelField<long> T::*>(fieldData.field) =
        std::stol(value);
  } else if (fieldData.ctype == typeid(unsigned long).name()) {
    instance.*std::any_cast<ModelField<unsigned long> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(short).name()) {
    instance.*std::any_cast<ModelField<short> T::*>(fieldData.field) =
        std::stoi(value);
  } else if (fieldData.ctype == typeid(char).name()) {
    instance.*std::any_cast<ModelField<char> T::*>(fieldData.field) =
        std::stoi(value);
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
std::string Model<T>::getFieldValue(T &instance,
                                    ModelDataField &fieldData) {
  if (fieldData.ctype == typeid(std::string).name()) {
    return instance
        .*std::any_cast<ModelField<std::string> T::*>(fieldData.field);
  } else if (fieldData.ctype == typeid(int).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<int> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(float).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<float> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(double).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<double> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(bool).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<bool> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long long).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<long long> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long long).name()) {
    return std::to_string(instance
                              .*std::any_cast<ModelField<unsigned long long> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned int).name()) {
    return std::to_string(instance
                              .*std::any_cast<ModelField<unsigned int> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned short).name()) {
    return std::to_string(instance
                              .*std::any_cast<ModelField<unsigned short> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned char).name()) {
    return std::to_string(instance
                              .*std::any_cast<ModelField<unsigned char> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(long).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<long> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long).name()) {
    return std::to_string(instance
                              .*std::any_cast<ModelField<unsigned long> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(short).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<short> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(char).name()) {
    return std::to_string(
        instance.*std::any_cast<ModelField<char> T::*>(fieldData.field));
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
BaseModelField *Model<T>::getField(T &instance, ModelDataField &fieldData) {
  if (fieldData.ctype == typeid(std::string).name()) {
    return &(instance
        .*std::any_cast<ModelField<std::string>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(int).name()) {
    return &(instance.*std::any_cast<ModelField<int>
                                     T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(float).name()) {
    return &(instance.*std::any_cast<ModelField<float>
                                     T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(double).name()) {
    return &(instance.*std::any_cast<ModelField<double>
                                     T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(bool).name()) {
    return &(instance.*std::any_cast<ModelField<bool>
                                     T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long long).name()) {
    return &(instance
        .*std::any_cast<ModelField<long long>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long long).name()) {
    return &(instance
        .*std::any_cast<ModelField<unsigned long long>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned int).name()) {
    return &(instance
        .*std::any_cast<ModelField<unsigned int>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned short).name()) {
    return &(instance
        .*std::any_cast<ModelField<unsigned short>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned char).name()) {
    return &(instance
        .*std::any_cast<ModelField<unsigned char>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long).name()) {
    return &(instance.*std::any_cast<ModelField<long>
                                     T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long).name()) {
    return &(instance
        .*std::any_cast<ModelField<unsigned long>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(short).name()) {
    return &(instance.*std::any_cast<ModelField<short>
                                     T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(char).name()) {
    return &(instance.*std::any_cast<ModelField<char>
                                     T::*>(fieldData.field));
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
bool Model<T>::operator==(const Model<T> &other) const {
  const ModelData &data = ModelManager::getInstance().getModel<T>();
  for (auto &fieldPair : data.fields) {
    auto &fieldData = const_cast<ModelDataField &>(fieldPair.second);
    if (getFieldValue(*static_cast<T *>(const_cast<Model<T> *>(this)),
                      fieldData) !=
        getFieldValue(*static_cast<T *>(const_cast<Model<T> *>(&other)),
                      fieldData)) {
      return false;
    }
  }
  return true;
}

} // ORM