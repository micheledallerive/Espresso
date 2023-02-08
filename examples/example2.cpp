//
// Created by michele on 19.01.23.
//
#include <iostream>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>
#include <orm/sql/SQLiteSQLGenerator.h>
#include "orm/model/ModelManager.h"
#include "orm/model/Model.h"

using namespace Espresso::ORM;

class Dog : public Model<Dog> {
 public:
  Dog() = default;
  ~Dog() = default;

  std::string name;
  std::string breed;
  int age{};
};

int main() {
  using std::make_pair;
  dbManager = std::make_shared<SQLiteDatabaseManager>();
  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  dbManager->connect(options);

  ModelManager::getInstance().registerModel<Dog>(
      "dogs",
      make_pair("name", &Dog::name),
      make_pair("breed", &Dog::breed),
      make_pair("age", &Dog::age)
  );

  Dog dog = Dog::get({{"name", "Loredana"}, {"breed", "Labrador"}});
  std::cout << dog.age << std::endl;

  dbManager->disconnect();
  return 0;
}