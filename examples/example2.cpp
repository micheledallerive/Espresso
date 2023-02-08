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

  std::cout << typeid(&User::name).name() << std::endl;
  ModelManager::registerModel<User>("test",
                                    std::make_pair("name", &User::name)
      //std::make_pair("age", &User::age)
  );
  std::cout << &User::name << std::endl;
  User u = User::get(2);
  std::cout << "Name: " << u.name << std::endl;

  dbManager->disconnect();
  return 0;
}