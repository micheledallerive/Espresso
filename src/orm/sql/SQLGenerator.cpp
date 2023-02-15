//
// Created by michele on 19.01.23.
//

#include "SQLGenerator.h"
#include "orm/model/query/filter/FilterOperation.h"
#include "orm/model/fields/FieldData.h"
#include <sstream>
#include <unordered_map>

namespace Espresso::ORM {

std::string SQLGenerator::createTable(const std::string &table_name,
                                      const std::vector<BaseFieldData *> &columns) {
  std::ostringstream sql;
  sql << "CREATE TABLE " << table_name << " (";
  // add columns with name type and primary key
  for (size_t i = 0; i < columns.size(); i++) {
    sql << columns[i]->name << " " << to_string(getSQLType(columns[i]->ctype));
    if (columns[i]->primaryKey) {
      sql << " PRIMARY KEY";
    }
    if (columns[i]->autoIncrement) {
      sql << " AUTOINCREMENT";
    }
    if (columns[i]->notNull) {
      sql << " NOT NULL";
    }
    if (columns[i]->defaultValue.has_value()) {
      sql << " DEFAULT " << columns[i]->defaultValue.value();
    }
    if (columns[i]->unique) {
      sql << " UNIQUE";
    }
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  for (const auto &column : columns) {
    auto *fk = dynamic_cast<ForeignKeyFieldData *>(column);
    if (fk == nullptr) {
      continue;
    }
    if (fk->foreignKey.has_value()) {
      sql << ", FOREIGN KEY (" << fk->name << ") REFERENCES "
          << fk->foreignKey->table << "("
          << fk->foreignKey->tablePrimaryKey << ")";
    }
  }
  sql << ");";
  return sql.str();
}

std::string SQLGenerator::dropTable(const std::string &table_name,
                                    bool ifExists) {
  std::ostringstream sql;
  sql << "DROP TABLE ";
  if (ifExists) {
    sql << "IF EXISTS ";
  }
  sql << table_name << ";";
  return sql.str();
}

std::string SQLGenerator::insert(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &values) {
  std::ostringstream sql;
  sql << "INSERT INTO " << table_name << " (";
  for (size_t i = 0; i < columns.size(); i++) {
    sql << columns[i];
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  sql << ") VALUES (";
  for (size_t i = 0; i < values.size(); i++) {
    sql << "'" << values[i] << "'";
    if (i < values.size() - 1) {
      sql << ", ";
    }
  }
  sql << ");";
  return sql.str();
}

std::string SQLGenerator::select(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::optional<Query::FilterOperation> &constraints) {
  std::ostringstream sql;
  sql << "SELECT ";
  if (columns.empty()) {
    sql << "*";
  } else {
    for (size_t i = 0; i < columns.size(); i++) {
      sql << columns[i];
      if (i < columns.size() - 1) {
        sql << ", ";
      }
    }
  }
  sql << " FROM " << table_name;
  if (constraints.has_value()) {
    sql << " WHERE " << constraints.value().toString();
  }
  sql << ";";
  return sql.str();
}

std::string SQLGenerator::update(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &values,
                                 const ConstraintMap &constraints) {
  std::ostringstream sql;
  sql << "UPDATE " << table_name << " SET ";
  for (size_t i = 0; i < columns.size(); ++i) {
    sql << columns[i] << "='" << values[i] << "'";
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  sql << " WHERE ";
  size_t i = 0;
  for (const auto &pair : constraints) {
    sql << pair.first << "='" << pair.second << "'";
    if (i < constraints.size() - 1) {
      sql << " AND ";
    }
    ++i;
  }
  sql << ";";
  return sql.str();
}
std::string SQLGenerator::alterTable(const std::string &table_name,
//                                     const std::vector<SQLColumnInfo> &toAdd,
//                                     const std::vector<SQLColumnInfo> &toRemove,
//                                     const std::vector<SQLColumnInfo> &toModify,
                                     const std::vector<BaseFieldData *> &intersectionColumns,
                                     const std::vector<BaseFieldData *> &modelColumns) {
  // write the sql statement for sqlite to alter the table
  std::ostringstream sql;
  // create a new table with a temporary name and modelColumns fields
  // copy the data from the old table to the new one
  // drop the old table
  // rename the new table to the old table name

  // first of all drop the temporary table if it exists
  sql << dropTable(table_name + "_temp", true);

  sql << "ALTER TABLE " << table_name << " RENAME TO " << table_name
      << "_temp;";

  sql << createTable(table_name, modelColumns);

  sql << "INSERT INTO " << table_name << " SELECT ";
  for (size_t i = 0; i < intersectionColumns.size(); i++) {
    sql << intersectionColumns[i]->name;
    if (i < intersectionColumns.size() - 1) {
      sql << ", ";
    }
  }
  if (intersectionColumns.size() < modelColumns.size()) {
    sql << ", 0 ";
  }
  sql << " FROM " << table_name << "_temp;";

  sql << dropTable(table_name + "_temp");
  return sql.str();
}

std::string SQLGenerator::remove(const std::string &table_name,
                                 const ConstraintMap &constraints) {
  std::ostringstream sql;
  sql << "DELETE FROM " << table_name;
  if (!constraints.empty()) {
    sql << " WHERE ";
    size_t i = 0;
    for (const auto &pair : constraints) {
      sql << pair.first << "='" << pair.second << "'";
      if (i < constraints.size() - 1) {
        sql << " AND ";
      }
      i++;
    }
  }
  sql << ";";
  return sql.str();
}

} // Espresso