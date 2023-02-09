//
// Created by michele on 19.01.23.
//
#include <iostream>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>
#include <orm/sql/SQLiteSQLGenerator.h>
#include "orm/model/ModelManager.h"
#include "orm/model/Model.h"
#include "orm/model/fields/ForeignKey.h"

using namespace Espresso::ORM;

class Dog : public Model<Dog> {
 public:
  Dog() = default;
  ~Dog() = default;

  ModelField<std::string> name;
  ModelField<std::string> breed;
  ModelField<int> age;

  PrimaryKey<int> pk;
  //ForeignKey<Dog> mother;
};

int main() {
  using std::make_pair;

  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  DatabaseManagerFactory<SQLiteDatabaseManager>::createAndConnect(options);

  ModelManager::getInstance().registerModel<Dog>(
      "dogs",
      make_pair("name", &Dog::name),
      make_pair("breed", &Dog::breed),
      make_pair("age", &Dog::age),
      make_pair("pk", &Dog::pk)
  );

  Dog dog;
  dog.name = "Loredana";
  dog.breed = "Pitbull";
  dog.age = 3;
  dog.save();

  dog.age = 5;
  dog.save();

  Dog dog2 = Dog::get({{"name", "Loredana"}});
  std::cout << dog2.age << std::endl;

//  Dog dog = Dog::get({{"name", "Loredana"}});
//  std::cout << dog.age << std::endl;

  dbManager->disconnect();
  return 0;
}