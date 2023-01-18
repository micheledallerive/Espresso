//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_

#include <sqlite3.h>
#include "DatabaseManager.h"

namespace Espresso {

class SQLiteConnectionOptions : private ConnectionOptions {
 public:
  ~SQLiteConnectionOptions() override = default;
  std::string databasePath;
};

class SQLiteDatabaseManager : public DatabaseManager {
 public:
  void connect(const ConnectionOptions &options) override;
  void disconnect() override;
  void execute(const std::string &query) override;
 private:
  sqlite3 *db;
};

}

#endif //ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_
