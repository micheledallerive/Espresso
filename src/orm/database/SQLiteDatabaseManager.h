//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "DatabaseManager.h"

namespace Espresso::ORM {

class SQLiteConnectionOptions : public ConnectionOptions {
 public:
  ~SQLiteConnectionOptions() override = default;
  std::string databasePath;
};

class SQLiteDatabaseManager : public DatabaseManager {
 public:
  void connect(const ConnectionOptions &options) override;
  void disconnect() override;
  void execute(const std::string &query, QueryCallback callback) override;

  std::vector<BaseFieldData*> getTableFields(const std::string &tableName) override;

  void startTransaction() override;
  void commitTransaction() override;
  void rollbackTransaction() override;

  void startSavepoint(const std::string &) override;
  void releaseSavepoint(const std::string &) override;
  void rollbackSavepoint(const std::string &) override;
 private:
  sqlite3 *db{nullptr};
};

}

#endif //ESPRESSO_SRC_ORM_DATABASE_SQLITEDATABASEMANAGER_H_
