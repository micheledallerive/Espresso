//
// Created by michele on 19.01.23.
//

#include "SQLGenerator.h"

namespace Espresso {

std::string SQLGenerator::createTable(const std::string &table_name,
                                      const std::vector<std::string> &columns,
                                      const std::vector<std::string> &types,
                                      const std::vector<std::string> &constraints) {
  std::string sql = "CREATE TABLE " + table_name + " (";
  for (int i = 0; i < columns.size(); i++) {
    sql += columns[i] + " " + types[i];
    if (i < constraints.size()) {
      sql += " " + constraints[i];
    }
    if (i < columns.size() - 1) {
      sql += ", ";
    }
  }
  sql += ");";
  return sql;
}

std::string SQLGenerator::dropTable(const std::string &table_name) {
  return "DROP TABLE " + table_name + ";";
}

std::string SQLGenerator::insert(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &values) {
  std::string sql = "INSERT INTO " + table_name + " (";
  for (int i = 0; i < columns.size(); i++) {
    sql += columns[i];
    if (i < columns.size() - 1) {
      sql += ", ";
    }
  }
  sql += ") VALUES (";
  for (int i = 0; i < values.size(); i++) {
    sql += values[i];
    if (i < values.size() - 1) {
      sql += ", ";
    }
  }
  sql += ");";
  return sql;
}

std::string SQLGenerator::select(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &constraints) {
  std::string sql = "SELECT ";
  if (columns.empty()) {
    sql += "*";
  } else {
    for (int i = 0; i < columns.size(); i++) {
      sql += columns[i];
      if (i < columns.size() - 1) {
        sql += ", ";
      }
    }
  }
  sql += " FROM " + table_name;
  if (!constraints.empty()) {
    sql += " WHERE ";
    for (int i = 0; i < constraints.size(); i++) {
      sql += constraints[i];
      if (i < constraints.size() - 1) {
        sql += " AND ";
      }
    }
  }
  sql += ";";
  return sql;
}

} // Espresso