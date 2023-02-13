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
template<class R>
Query::QueryBuilder<R> Model<T>::related(const std::string &relatedName) {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  ModelData &relatedData = ModelManager::getInstance().getModel<R>();

  if (!data.relatedModels.contains(relatedName)) {
    throw model_error(
        "Table " + data.tableName + " has no related name " + relatedName);
  }

  std::pair<std::string, std::string>
      related = data.relatedModels[relatedName];
  std::string relatedModel = related.first;
  std::string fkField = related.second;

  if (relatedModel != typeid(R).name()) {
    throw model_error("Related model " + relatedModel + " is not of type " +
        typeid(R).name());
  }

  return R::all().filter(Query::Q(fkField) == this->get(data.primaryKey));
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
  std::string primaryKeyValue = instance->get(primaryKeyField);
  string query = SQLGenerator::remove(data.tableName,
                                      {{primaryKeyField, primaryKeyValue}});
  dbManager->execute(query);
  return true;
}

template<class T>
void Model<T>::save(bool checkDirty) {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  T *instance = static_cast<T *>(this);

  std::vector<string> fields;
  std::vector<string> values;
  for (auto &modelField : data.fields) {
    BaseModelField *baseField = instance->getField(modelField.first);
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

    if (checkDirty && !dirty) continue;

    fields.push_back(modelField.first);
    values.push_back(instance->get(modelField.first));
  }

  if (!wasSaved) { // instance does not exist: insert
    string query = SQLGenerator::insert(data.tableName, fields, values);
    dbManager->execute(query,
                       [&instance, &data](std::unordered_map<std::string,
                                                             std::string> &result) {
                         if (result.contains("id") && !data.primaryKey.empty())
                           instance->set(data.primaryKey, result["id"]);
                       });
    wasSaved = true;
  } else { // already exists: update it
    std::string
        primaryKeyField = ModelManager::getInstance().getModel<T>().primaryKey;
    std::string primaryKeyValue = instance->get(primaryKeyField);
    string query = SQLGenerator::update(data.tableName,
                                        fields,
                                        values,
                                        {{primaryKeyField, primaryKeyValue}});
    dbManager->execute(query);
  }
}

template<class T>
ModelDataField &Model<T>::getFieldData(const std::string &fieldName) {
  return ModelManager::getInstance().getModel<T>().fields[fieldName];
}

template<class T>
void Model<T>::set(const std::string &field, const std::string &value) {
  const ModelDataField &fieldData = getFieldData(field);
  if (fieldData.ctype == typeid(std::string).name()) {
    (T &) *this.*std::any_cast<ModelField<std::string> T::*>(fieldData.field) =
        value;
  } else if (fieldData.ctype == typeid(int).name()) {
    (T &) *this.*std::any_cast<ModelField<int> T::*>(fieldData.field) =
        std::stoi(value);
  } else if (fieldData.ctype == typeid(float).name()) {
    (T &) *this.*std::any_cast<ModelField<float> T::*>(fieldData.field) =
        std::stof(value);
  } else if (fieldData.ctype == typeid(double).name()) {
    (T &) *this.*std::any_cast<ModelField<double> T::*>(fieldData.field) =
        std::stod(value);
  } else if (fieldData.ctype == typeid(bool).name()) {
    (T &) *this.*std::any_cast<ModelField<bool> T::*>(fieldData.field) =
        std::stoi(value);
  } else if (fieldData.ctype == typeid(long long).name()) {
    (T &) *this.*std::any_cast<ModelField<long long> T::*>(fieldData.field) =
        std::stoll(value);
  } else if (fieldData.ctype == typeid(unsigned long long).name()) {
    (T &) *this
        .*std::any_cast<ModelField<unsigned long long> T::*>(fieldData.field) =
        std::stoull(value);
  } else if (fieldData.ctype == typeid(unsigned int).name()) {
    (T &) *this.*std::any_cast<ModelField<unsigned int> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(unsigned short).name()) {
    (T &) *this
        .*std::any_cast<ModelField<unsigned short> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(unsigned char).name()) {
    (T &) *this
        .*std::any_cast<ModelField<unsigned char> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(long).name()) {
    (T &) *this.*std::any_cast<ModelField<long> T::*>(fieldData.field) =
        std::stol(value);
  } else if (fieldData.ctype == typeid(unsigned long).name()) {
    (T &) *this
        .*std::any_cast<ModelField<unsigned long> T::*>(fieldData.field) =
        std::stoul(value);
  } else if (fieldData.ctype == typeid(short).name()) {
    (T &) *this.*std::any_cast<ModelField<short> T::*>(fieldData.field) =
        std::stoi(value);
  } else if (fieldData.ctype == typeid(char).name()) {
    (T &) *this.*std::any_cast<ModelField<char> T::*>(fieldData.field) =
        std::stoi(value);
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
std::string Model<T>::get(const std::string &field) const {
  const ModelDataField &fieldData = getFieldData(field);
  if (fieldData.ctype == typeid(std::string).name()) {
    return (T &) *this
        .*std::any_cast<ModelField<std::string> T::*>(fieldData.field);
  } else if (fieldData.ctype == typeid(int).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<int> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(float).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<float> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(double).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<double> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(bool).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<bool> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long long).name()) {
    return std::to_string(
        (T &) *this
            .*std::any_cast<ModelField<long long> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long long).name()) {
    return std::to_string((T &) *this
                              .*std::any_cast<ModelField<unsigned long long> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned int).name()) {
    return std::to_string((T &) *this
                              .*std::any_cast<ModelField<unsigned int> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned short).name()) {
    return std::to_string((T &) *this
                              .*std::any_cast<ModelField<unsigned short> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned char).name()) {
    return std::to_string((T &) *this
                              .*std::any_cast<ModelField<unsigned char> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(long).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<long> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long).name()) {
    return std::to_string((T &) *this
                              .*std::any_cast<ModelField<unsigned long> T::*>(
                                  fieldData.field));
  } else if (fieldData.ctype == typeid(short).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<short> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(char).name()) {
    return std::to_string(
        (T &) *this.*std::any_cast<ModelField<char> T::*>(fieldData.field));
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
BaseModelField *Model<T>::getField(const std::string &field) {
  const auto &fieldData = this->getFieldData(field);
  if (fieldData.ctype == typeid(std::string).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<std::string> T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(int).name()) {
    return &((T &) *this.*std::any_cast<ModelField<int>
                                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(float).name()) {
    return &((T &) *this.*std::any_cast<ModelField<float>
                                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(double).name()) {
    return &((T &) *this.*std::any_cast<ModelField<double>
                                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(bool).name()) {
    return &((T &) *this.*std::any_cast<ModelField<bool>
                                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long long).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<long long>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long long).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<unsigned long long>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned int).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<unsigned int>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned short).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<unsigned short>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned char).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<unsigned char>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long).name()) {
    return &((T &) *this.*std::any_cast<ModelField<long>
                                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(unsigned long).name()) {
    return &((T &) *this
        .*std::any_cast<ModelField<unsigned long>
                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(short).name()) {
    return &((T &) *this.*std::any_cast<ModelField<short>
                                        T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(char).name()) {
    return &((T &) *this.*std::any_cast<ModelField<char>
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
    if (this->get(fieldPair.first) != other.get(fieldPair.first)) {
      return false;
    }
  }
  return true;
}

} // ORM