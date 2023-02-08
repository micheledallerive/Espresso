//
// Created by michele on 19.01.23.
//
#include <iostream>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>
#include "orm/model/ModelManager.h"
#include "orm/model/Model.h"

using namespace Espresso::ORM;

class User : public Model<User> {
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

  ModelManager::getInstance().registerModel<User>("test",
                                    std::make_pair("name", &User::name),
                                    std::make_pair("age", &User::age)
  );
  User u = User::get(1);

  std::cout << "Name: " << u.name << std::endl;
  std::cout << "Age: " << u.age << std::endl;

  u.age = 69;
  u.save();

  User u2 = User();
  u2.name = "provone";
  u2.age = 420;
  u2.save();

  dbManager->disconnect();
  return 0;
}