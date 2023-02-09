//
// Created by michele on 19.01.23.
//
#include <iostream>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>
#include <orm/sql/SQLiteSQLGenerator.h>
#include "orm/model/ModelManager.h"
#include "orm/model/Model.h"
#include "orm/utils/PrimitiveWrapper.h"

using namespace Espresso::ORM;

class Dog : public Model<Dog> {
 public:
  Dog() = default;
  ~Dog() = default;

  ModelField<std::string> name;
  ModelField<std::string> breed;
  ModelField<int> age;
  PrimaryKey<int> pk;
  PrimaryKey<int> pk2;
};

int main() {
  using std::make_pair;

  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  DatabaseManagerFactory<SQLiteDatabaseManager>::createAndConnect(options);

  ModelManager::getInstance().registerModel<Dog>(
      "dogs",
      make_pair("pk", &Dog::pk),
      make_pair("pk2", &Dog::pk),
      make_pair("name", &Dog::name),
      make_pair("breed", &Dog::breed)
  );

//  Dog dog = Dog::get({{"name", "Loredana"}});
//  std::cout << dog.age << std::endl;

  dbManager->disconnect();
  return 0;
}