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

#include <orm/model/query/filter/FilterField.h>
#include <orm/model/query/filter/FilterNode.h>

#include <orm/model/query/QueryBuilder.h>

using namespace Espresso::ORM;
using namespace Espresso::ORM::Query;
using namespace std;

class Human : public Model<Human> {
 public:
  Human() = default;
  ~Human() = default;

  PrimaryKey<int> pk;

  ModelField<std::string> name;
  ModelField<std::string> surname;
  ModelField<int> age;
};

class Dog : public Model<Dog> {
 public:
  Dog() = default;
  ~Dog() override = default;

  Dog(const std::string &name, const std::string &breed, int age)
      : name(name), breed(breed), age(age) {}

  PrimaryKey<int> pk;

  ModelField<std::string> name;
  ModelField<std::string> breed;
  ModelField<int> age;

  ForeignKey<Human> owner;
};

void registerModels() {

  ModelManager::getInstance().registerModel<Human>(
      "humans",
      make_pair(Field{.name="pk", .autoIncrement=true}, &Human::pk),
      make_pair(Field{.name="name"}, &Human::name),
      make_pair(Field{.name="surname"}, &Human::surname),
      make_pair(Field{.name="age"}, &Human::age)
  );
  ModelManager::getInstance().registerModel<Dog>(
      "dogs",
      make_pair(Field{.name="pk", .autoIncrement=true}, &Dog::pk),
      make_pair(Field{.name="name"}, &Dog::name),
      make_pair(Field{.name="breed"}, &Dog::breed),
      make_pair(Field{.name="age"}, &Dog::age),
      make_pair(Field{.name="owner"}, &Dog::owner)
  );
}

int main() {
  using std::make_pair;

  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  DatabaseManagerFactory<SQLiteDatabaseManager>::createAndConnect(options);

  registerModels();

  std::vector<Dog> dogs = Dog::all();

  for (Dog &dog : dogs) {
    std::cout << dog.name << std::endl;
  }

  std::cout << "----------------" << std::endl;

  std::cout << "Count: " << Dog::all().count() << std::endl;

  Dog newDog = Dog::all().create("a", "b", 5);

//  Dog d;
//  try {
//    d = Dog::get({{"name", "Ciccio"}});
//  } catch (const std::exception &e) {
//    d.name = "Ciccio";
//    d.breed = "Labrador";
//    d.age = 3;
//    d.save();
//  }
//
//  Human h;
//  try {
//    h = Human::get({{"name", "Michele"}});
//  } catch (const std::exception &e) {
//    h.name = "Michele";
//    h.surname = "Dalle Rive";
//    h.age = 23;
//    h.save();
//  }
//
//  std::optional<Human *> h2 = *d.owner;
//  if (h2.has_value()) {
//    std::cout << h2.value()->name << std::endl;
//  } else {
//    std::cout << "No owner" << std::endl;
//  }


//  Dog dog = Dog::get({{"name", "Loredana"}});
//  std::cout << dog.age << std::endl;

  dbManager->disconnect();
  return 0;
}