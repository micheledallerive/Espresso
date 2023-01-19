//
// Created by michele on 18.01.23.
//

#include "SQLiteDatabaseManager.h"
#include <sqlite3.h>
#include "../exceptions.h"

namespace Espresso {

void SQLiteDatabaseManager::connect(const ConnectionOptions &options) {
  auto sqliteOptions = dynamic_cast<const SQLiteConnectionOptions &>(options);
  sqlite3_open(sqliteOptions.databasePath.c_str(), &db);
  if (db == nullptr) {
    throw db_error("Could not connect to database");
  }

  connected = true;
}

void SQLiteDatabaseManager::disconnect() {
  sqlite3_close(db);
}

void SQLiteDatabaseManager::execute(const std::string &query) {
  const char *error;
  sqlite3_exec(db, query.c_str(), nullptr, nullptr, (char **) &error);
  if (error != nullptr) {
    throw Espresso::sql_error(error);
  }
}

}