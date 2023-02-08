//
// Created by michele on 08.02.23.
//

#include "Model.h"
#include <orm/database/DatabaseManager.h>
#include <orm/model/ModelManager.h>
#include <iostream>

namespace Espresso::ORM {

template<class T>
T Model<T>::get(int id) {
  ModelData &data = ModelManager::models[typeid(T).name()];
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
  return instance;
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

} // ORM