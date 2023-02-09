//
// Created by michele on 08.02.23.
//

#include "Model.h"
#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <orm/sql/SQLGenerator.h>
#include <orm/exceptions.h>

#include <iostream>

namespace Espresso::ORM {

template<class T>
T Model<T>::get(ConstraintMap constraints) {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  string query = SQLGenerator::select(data.tableName, {"*"}, constraints);
  T instance;
  bool found = false;
  dbManager->execute(query,
                     [&data, &instance, &found](std::unordered_map<std::string,
                                                                   std::string> &result) {
                       if (!data.fields.empty()) found = true;
                       for (auto &ModelDataField : data.fields) {
                         setField(instance,
                                  ModelDataField.second,
                                  result[ModelDataField.first]);
                       }
                     });
  if (!found) {
    throw object_not_found("Object not found");
  }
  return instance;
}

template<class T>
void Model<T>::save() {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  T *instance = static_cast<T *>(this);

  std::vector<string> fields;
  std::vector<string> values;
  for (auto &modelField : data.fields) {
    fields.push_back(modelField.first);
    values.push_back(getField(*instance, modelField.second));
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
std::string Model<T>::getField(T &instance,
                               const ModelDataField &fieldData) {
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

} // ORM