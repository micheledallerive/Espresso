//
// Created by michele on 19.01.23.
//

#ifndef ESPRESSO_SRC_ORM_MODELS_SQLGENERATOR_H_
#define ESPRESSO_SRC_ORM_MODELS_SQLGENERATOR_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <orm/sql/SQLTypes.h>
#include "orm/model/query/filter/FilterOperation.h"
#include "orm/model/fields/FieldData.h"

namespace Espresso::ORM {

class SQLGenerator {
 public:
  SQLGenerator() = delete;
  SQLGenerator(const SQLGenerator &) = delete;
  SQLGenerator(SQLGenerator &&) = delete;
  SQLGenerator &operator=(const SQLGenerator &) = delete;
  SQLGenerator &operator=(SQLGenerator &&) = delete;

  static std::string createTable(const std::string &,
                                 const std::vector<BaseFieldData*> &);
  static std::string alterTable(const std::string &,
//                                const std::vector<SQLColumnInfo> &toAdd,
//                                const std::vector<SQLColumnInfo> &toRemove,
//                                const std::vector<SQLColumnInfo> &toModify,
                                const std::vector<BaseFieldData *> &intersectionColumns,
                                const std::vector<BaseFieldData *> &modelColumns);

  static std::string dropTable(const std::string &table_name,
                               bool ifExists = false);
  static std::string insert(const std::string &table_name,
                            const std::vector<std::string> &columns,
                            const std::vector<std::string> &values);
  static std::string select(const std::string &table_name,
                            const std::vector<std::string> &columns,
                            const std::optional<Query::FilterOperation> &);
  static std::string update(const std::string &table_name,
                            const std::vector<std::string> &columns,
                            const std::vector<std::string> &values,
                            const ConstraintMap &constraints);
  static std::string remove(const std::string &table_name,
                            const ConstraintMap &constraints);

};

} // Espresso

#endif //ESPRESSO_SRC_ORM_MODELS_SQLGENERATOR_H_
