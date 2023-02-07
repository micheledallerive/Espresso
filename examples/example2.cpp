//
// Created by michele on 19.01.23.
//
#include <iostream>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>
#include "orm/model/ModelManager.h"

using namespace Espresso::ORM;

class User {
 public:
  User() = default;
  ~User() = default;

  std::string name;
  int age{};
};

int main() {
  dbManager = std::make_shared<SQLiteDatabaseManager>();
  Espresso::ORM::SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  dbManager->connect(options);

  std::string tableName = "users";
  ModelManager::registerModel<User>(tableName,
                                    std::make_pair("name", &User::name),
                                    std::make_pair("age", &User::age)
  );

  dbManager->disconnect();
  return 0;
}