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
struct Field {
  std::string name;
  bool primaryKey{false};
  bool autoIncrement{false};
};
class ModelDataField : public Field {
 public:
  std::any field;
  std::string ctype;

  ModelDataField() = default;
  // create a constructor for the assignment ModelDataField = Field
  // Field does not have a constructor
  ModelDataField(const Field &f) {
    this->name = f.name;
    this->primaryKey = f.primaryKey;
    this->autoIncrement = f.autoIncrement;
  }
};
struct ModelData {
  std::string tableName; // the name of the table for the model
  std::unordered_map<std::string, ModelDataField> fields; // field name and data
  bool migrated{false}; // whether the model was migrated
  // the name of the primary key (can be accessed by the field name in the map)
  std::string primaryKey;

  std::vector<Espresso::ORM::SQLColumnInfo> getColumns() const {
    std::vector<Espresso::ORM::SQLColumnInfo> columns;
    for (const auto &field : fields) {
      columns.emplace_back(field.first,
                           Espresso::ORM::getSQLType(field.second.ctype),
                           false,
                           field.second.primaryKey);
    }
    return columns;
  }
};
namespace Espresso::ORM {

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_MODELDATA_H_
