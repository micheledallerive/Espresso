//
// Created by michele on 19.01.23.
//

#include "SQLGenerator.h"
#include <sstream>
#include <unordered_map>

namespace Espresso::ORM {

std::string SQLGenerator::createTable(const std::string &table_name,
                                      const std::vector<std::string> &columns,
                                      const std::vector<std::string> &types,
                                      const ConstraintMap &constraints) {
  std::ostringstream sql;
  sql << "CREATE TABLE " << table_name << " (";
  for (int i = 0; i < columns.size(); i++) {
    sql << columns[i] << " " << types[i];
    if (constraints.count(columns[i])) {
      sql << " " << constraints.at(columns[i]);
    }
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  sql << ");";
  return sql.str();
}

std::string SQLGenerator::dropTable(const std::string &table_name) {
  return "DROP TABLE " + table_name + ";";
}

std::string SQLGenerator::insert(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &values) {
  std::ostringstream sql;
  sql << "INSERT INTO " << table_name << " (";
  for (int i = 0; i < columns.size(); i++) {
    sql << columns[i];
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  sql << ") VALUES (";
  for (int i = 0; i < values.size(); i++) {
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
                                 const ConstraintMap &constraints) {
  std::ostringstream sql;
  sql << "SELECT ";
  if (columns.empty()) {
    sql << "*";
  } else {
    for (int i = 0; i < columns.size(); i++) {
      sql << columns[i];
      if (i < columns.size() - 1) {
        sql << ", ";
      }
    }
  }
  sql << " FROM " << table_name;
  if (!constraints.empty()) {
    sql << " WHERE ";
    int i = 0;
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

std::string SQLGenerator::update(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &values,
                                 const ConstraintMap &constraints) {
  std::ostringstream sql;
  sql << "UPDATE " << table_name << " SET ";
  for (int i = 0; i < columns.size(); ++i) {
    sql << columns[i] << "='" << values[i] << "'";
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }
  sql << " WHERE ";
  int i = 0;
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
                                     const std::vector<SQLColumnInfo> &toAdd,
                                     const std::vector<SQLColumnInfo> &toRemove,
                                     const std::vector<SQLColumnInfo> &toModify,
                                     const std::vector<SQLColumnInfo> &intersectionColumns,
                                     const std::vector<SQLColumnInfo> &modelColumns) {
  // write the sql statement for sqlite to alter the table
  std::ostringstream sql;
  // create a new table with a temporary name and modelColumns fields
  // copy the data from the old table to the new one
  // drop the old table
  // rename the new table to the old table name
  sql << "ALTER TABLE " << table_name << " RENAME TO " << table_name
      << "_temp;";
  std::vector<std::string> columns;
  std::vector<std::string> types;
  for (const auto &column : modelColumns) {
    columns.push_back(column.name);
    types.push_back(to_string(column.type));
  }
  sql << createTable(table_name, columns, types, {});
  sql << "INSERT INTO " << table_name << " SELECT ";
  for (int i = 0; i < intersectionColumns.size(); i++) {
    sql << intersectionColumns[i].name;
    if (i < intersectionColumns.size() - 1) {
      sql << ", ";
    }
  }
  sql << " FROM " << table_name << "_temp;";
  sql << dropTable(table_name + "_temp");
  return sql.str();
}

} // Espresso