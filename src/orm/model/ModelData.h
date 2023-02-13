//
// Created by michele on 10.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_MODELDATA_H_
#define ESPRESSO_SRC_ORM_MODEL_MODELDATA_H_

#include "orm/sql/SQLTypes.h"
#include <utility>
#include <shared_mutex>
#include <mutex>
#include <any>
#include <vector>
#include <unordered_map>
#include <string>
#include <orm/model/fields/Field.h>

namespace Espresso::ORM {

struct ModelData {
  std::string tableName; // the name of the table for the model
  std::unordered_map<std::string, ModelDataField> fields; // field name and data
  bool migrated{false}; // whether the model was migrated
  // the name of the primary key (can be accessed by the field name in the map)
  std::string primaryKey;

  // map [relationship name] -> [related model name, related model field name]
  // e.g. "dogs" -> ["Dog", "owner"]
  std::unordered_map<std::string, std::pair<std::string, std::string>>
      relatedModels;

  std::vector<Espresso::ORM::SQLColumnInfo> getColumns() const {
    std::vector<Espresso::ORM::SQLColumnInfo> columns;
    for (const auto &field : fields) {
      columns.emplace_back(field.second);
    }
    return columns;
  }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_MODELDATA_H_
