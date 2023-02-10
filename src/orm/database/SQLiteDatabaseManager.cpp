//
// Created by michele on 18.01.23.
//

#include "SQLiteDatabaseManager.h"
#include <sqlite3.h>
#include "../exceptions.h"
#include "utils.h"

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
  this->connected = false;
}

void SQLiteDatabaseManager::execute(const std::string &query,
                                    QueryCallback callback) {
  if (!this->connected) {
    throw db_error("Not connected to database");
  }
  auto pos = query.find_first_of(';');
  if (pos != std::string::npos && pos != query.size() - 1) {
    for (auto &q : Espresso::split(query, ';')) {
      execute(q, callback);
    }
    return;
  }

  sqlite3_stmt *stmt;
  const char *sql = query.c_str();
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw sql_error(
        "Could not prepare statement: '" + query + "': " + std::string(sqlite3_errmsg(db)));
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    std::unordered_map<std::string, std::string> rowResult;
    // for each column
    for (int i = 0; i < sqlite3_column_count(stmt); i++) {
      std::string columnName = sqlite3_column_name(stmt, i);
      const char *columnValue = (const char *) sqlite3_column_text(stmt, i);
      rowResult.emplace(columnName,
                        columnValue == nullptr ? "" : std::string(columnValue));
    }
    if (callback) callback(rowResult);
  }
  if (query.find("INSERT") != std::string::npos) {
    std::string last_id = std::to_string(sqlite3_last_insert_rowid(db));
    std::unordered_map<std::string, std::string> rowResult;
    rowResult.emplace("id", last_id);
    if (callback) callback(rowResult);
  }
  if (rc != SQLITE_DONE) {
    throw sql_error("Could not execute statement '" + query + "': " +
                    std::string(sqlite3_errmsg(db)));
  }
  sqlite3_finalize(stmt);
}

}