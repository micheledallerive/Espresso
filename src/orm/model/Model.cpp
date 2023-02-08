//
// Created by michele on 08.02.23.
//

#include "Model.h"
#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <iostream>

namespace Espresso::ORM {

template<class T>
T Model<T>::get(long long id) {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  string query =
      "SELECT * FROM " + data.tableName + " WHERE id = " + std::to_string(id);
  T instance;
  dbManager->execute(query,
                     [&data, &instance](std::unordered_map<std::string,
                                                           std::string> &result) {
                       for (auto &modelField : data.fields) {
                         setField(instance,
                                  modelField.second,
                                  result[modelField.first]);
                       }
                     });
  instance.id = id;
  return instance;
}

template<class T>
void Model<T>::save() {
  ModelData &data = ModelManager::getInstance().getModel<T>();
  T *instance = static_cast<T *>(this);
  if (instance->id == -1) {
    string query = "INSERT INTO " + data.tableName + " (";
    for (auto &modelField : data.fields) {
      query += modelField.first + ", ";
    }
    query.pop_back();
    query.pop_back();
    query += ") VALUES (";
    for (auto &modelField : data.fields) {
      query += "'" + getField(*instance, modelField.second) + "', ";
    }
    query.pop_back();
    query.pop_back();
    query += ")";
    dbManager->execute(query,
                       [&instance](std::unordered_map<std::string,
                                                      std::string> &result) {
                         instance->id = std::stoll(result["id"]);
                       });
  } else {
    string query = "UPDATE " + data.tableName + " SET ";
    for (auto &modelField : data.fields) {
      query +=
          modelField.first + " = '" + getField(*instance, modelField.second)
              + "', ";
    }
    query.pop_back();
    query.pop_back();
    query += " WHERE id = " + std::to_string(instance->id);
    dbManager->execute(query, nullptr);
  }
}

template<class T>
void Model<T>::setField(T &instance,
                        const ModelField &fieldData,
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
  } else {
    throw std::runtime_error("Unknown type");
  }
}

template<class T>
std::string Model<T>::getField(T &instance,
                               const ModelField &fieldData) {
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
  } else {
    throw std::runtime_error("Unknown type");
  }
}

} // ORM