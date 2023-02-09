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
using namespace std;

class Dog : public Model<Dog> {
 public:
  Dog() = default;
  ~Dog() = default;

  PrimaryKey<int> pk;
  ModelField<std::string> name;
  ModelField<std::string> breed;
  ModelField<int> age;
  //ForeignKey<Dog> mother;
};

int main() {
  using std::make_pair;

  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  DatabaseManagerFactory<SQLiteDatabaseManager>::createAndConnect(options);

  ModelManager::getInstance().registerModel<Dog>(
      "dogs",
      make_pair(Field{.name="pk", .primaryKey=true, .autoIncrement=true},
                &Dog::pk),
      make_pair(Field{.name="name"}, &Dog::name),
      make_pair(Field{.name="breed"}, &Dog::breed),
      make_pair(Field{.name="age"}, &Dog::age)
  );

  Dog dog;
  dog.name = "Loredana";
  dog.breed = "Pitbull";
  dog.age = 10;
  dog.save();

//  try {
//    Dog dog2 = Dog::get({{"name", "Loredana"}});
//    std::cout << dog2.age << std::endl;
//  } catch (std::exception &e) {
//    std::cerr << "Error: " << e.what() << std::endl;
//  }

//  Dog dog = Dog::get({{"name", "Loredana"}});
//  std::cout << dog.age << std::endl;

  dbManager->disconnect();
  return 0;
}