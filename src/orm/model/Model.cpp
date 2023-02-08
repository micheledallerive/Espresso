//
// Created by michele on 08.02.23.
//

#include "Model.h"
#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <iostream>
#include <orm/sql/SQLGenerator.h>

namespace Espresso::ORM {

template<class T>
T Model<T>::get(ConstraintMap constraints) {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  string query = SQLGenerator::select(data.tableName, {"*"}, constraints);
  T instance;
  dbManager->execute(query,
                     [&data, &instance](std::unordered_map<std::string,
                                                           std::string> &result) {
                       for (auto &ModelDataField : data.fields) {
                         setField(instance,
                                  ModelDataField.second,
                                  result[ModelDataField.first]);
                       }
                     });
  instance.id = id;
  return instance;
}

template<class T>
void Model<T>::save() {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  T *instance = static_cast<T *>(this);

  std::vector<string> fields;
  std::vector<string> values;
  for (auto &ModelDataField : data.fields) {
    fields.push_back(ModelDataField.first);
    values.push_back(getField(*instance, ModelDataField.second));
  }

  if (instance->id == -1) { // instance does not exist: insert
    string query = SQLGenerator::insert(data.tableName, fields, values);
    dbManager->execute(query,
                       [&instance](std::unordered_map<std::string,
                                                      std::string> &result) {
                         instance->id = std::stoll(result["id"]);
                       });
  } else { // already exists: update it todo improve this check lmao
    string query = SQLGenerator::update(data.tableName,
                                        fields,
                                        values,
                                        {{"id", std::to_string(instance->id)}});
    dbManager->execute(query, nullptr);
  }
}

template<class T>
void Model<T>::setField(T &instance,
                        const ModelDataField &fieldData,
                        const std::string &value) {
  if (fieldData.ctype == typeid(std::string).name()) {
    instance.*std::any_cast<std::string T::*>(fieldData.field) = value;
  } else if (fieldData.ctype == typeid(int).name()) {
    instance.*std::any_cast<int T::*>(fieldData.field) = std::stoi(value);
  } else if (fieldData.ctype == typeid(float).name()) {
    instance.*std::any_cast<float T::*>(fieldData.field) = std::stof(value);
  } else if (fieldData.ctype == typeid(double).name()) {
    instance.*std::any_cast<double T::*>(fieldData.field) = std::stod(value);
  } else if (fieldData.ctype == typeid(bool).name()) {
    instance.*std::any_cast<bool T::*>(fieldData.field) = std::stoi(value);
  } else if (fieldData.ctype == typeid(long long).name()) {
    instance.*std::any_cast<long long T::*>(fieldData.field) =
        std::stoll(value);
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
std::string Model<T>::getField(T &instance,
                               const ModelDataField &fieldData) {
  if (fieldData.ctype == typeid(std::string).name()) {
    return instance.*std::any_cast<std::string T::*>(fieldData.field);
  } else if (fieldData.ctype == typeid(int).name()) {
    return std::to_string(instance.*std::any_cast<int T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(float).name()) {
    return std::to_string(instance.*std::any_cast<float T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(double).name()) {
    return std::to_string(
        instance.*std::any_cast<double T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(bool).name()) {
    return std::to_string(instance.*std::any_cast<bool T::*>(fieldData.field));
  } else if (fieldData.ctype == typeid(long long).name()) {
    return std::to_string(
        instance.*std::any_cast<long long T::*>(fieldData.field));
  } else {
    throw std::runtime_error("Unknown type");
  }
}

} // ORM