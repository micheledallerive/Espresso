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
                         std::cout << modelField.first << " "
                                   << result[modelField.first] << std::endl;
                         auto fieldPtr =
                             std::any_cast<std::string T::*>(modelField.second.field);
                         instance.*(fieldPtr) = "espkere";
                       }
                     });
  return instance;
}

} // ORM