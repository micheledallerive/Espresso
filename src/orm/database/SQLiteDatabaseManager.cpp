//
// Created by michele on 18.01.23.
//

#include "SQLiteDatabaseManager.h"
#include <sqlite3.h>

namespace Espresso {

void SQLiteDatabaseManager::connect(const ConnectionOptions &options) {
  auto sqliteOptions = dynamic_cast<const SQLiteConnectionOptions &>(options);
  sqlite3_open(sqliteOptions.databasePath.c_str(), &db);
}

}