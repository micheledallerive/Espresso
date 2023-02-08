//
// Created by michele on 18.01.23.
//

#include "SQLiteDatabaseManager.h"
#include <sqlite3.h>
#include "../exceptions.h"

namespace Espresso::ORM {

void SQLiteDatabaseManager::connect(const ConnectionOptions &options) {
  auto sqliteOptions = dynamic_cast<const SQLiteConnectionOptions &>(options);
  sqlite3_open(sqliteOptions.databasePath.c_str(), &db);
  if (db == nullptr) {
    throw db_error("Could not connect to database");
  }

  this->connected = true;
}

void SQLiteDatabaseManager::disconnect() {
  sqlite3_close(db);
}

void SQLiteDatabaseManager::execute(const std::string &query,
                                    QueryCallback callback) {
  const char *error;
  sqlite3_stmt *stmt;
  const char *sql = query.c_str();
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw sql_error("Could not prepare statement");
    return;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    std::unordered_map<std::string, std::string> rowResult;
    // for each column
    for (int i = 0; i < sqlite3_column_count(stmt); i++) {
      std::string columnName = sqlite3_column_name(stmt, i);
      std::string columnValue = (const char *) sqlite3_column_text(stmt, i);
      rowResult.emplace(columnName, columnValue);
    }
    if (callback) callback(rowResult);
  }
  if (rc != SQLITE_DONE) {
    throw sql_error("Could not execute statement");
  }
  sqlite3_finalize(stmt);
}

}