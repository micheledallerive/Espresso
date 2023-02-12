//
// Created by michele on 19.01.23.
//

#include "SQLGenerator.h"
#include "orm/model/query/filter/FilterNode.h"
#include <sstream>
#include <unordered_map>

namespace Espresso::ORM {

std::string SQLGenerator::createTable(const std::string &table_name,
                                      const std::vector<SQLColumnInfo> &columns) {
  std::ostringstream sql;
  sql << "CREATE TABLE " << table_name << " (";
  // add columns with name type and primary key
  for (size_t i = 0; i < columns.size(); i++) {
    sql << columns[i].name << " " << to_string(columns[i].type);
    if (columns[i].primaryKey) {
      sql << " PRIMARY KEY";
    }
    if (columns[i].autoIncrement) {
      sql << " AUTOINCREMENT";
    }
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  for (const auto &column : columns) {
    if (column.foreignKey.has_value()) {
      sql << ", FOREIGN KEY (" << column.name << ") REFERENCES "
          << column.foreignKey->table << "("
          << column.foreignKey->tablePrimaryKey << ")";
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
                                 Query::FilterNode *constraints) {
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
  if (constraints != nullptr) {
    sql << " WHERE " << constraints->toString();
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
                                     const std::vector<SQLColumnInfo> &intersectionColumns,
                                     const std::vector<SQLColumnInfo> &modelColumns) {
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
    sql << intersectionColumns[i].name;
    if (i < intersectionColumns.size() - 1) {
      sql << ", ";
    }
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