//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include "DatabaseManager.h"

namespace Espresso {

class SQLiteConnectionOptions : public ConnectionOptions {
 public:
  ~SQLiteConnectionOptions() override = default;
  std::string databasePath;
};

class SQLiteDatabaseManager : public DatabaseManager {
 public:
  void connect(const ConnectionOptions &options) override;
  void disconnect() override;
  void execute(const std::string &query) override;

  template<class T>
  void registerModel(const std::string &tableName,
                     const std::vector<std::pair<std::string,
                                                 std::string T::*>>
                     &fields);

  bool connected = false;
 private:
  sqlite3 *db{nullptr};
};

}

#endif //ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_
