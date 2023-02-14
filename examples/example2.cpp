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

class Cat : public Model<Cat> {
 public:
  Cat() = default;
  ~Cat() override = default;

  Cat(const std::string &name, const std::string &breed, int age)
      : name(name), breed(breed), age(age) {}

  PrimaryKey<int> pk;

  ModelField<std::string> name;
  ModelField<std::string> breed;
  ModelField<int> age;
};

void registerModels() {

  ModelManager::getInstance().registerModel<Human>(
      "humans",
      make_pair(FieldParams("pk", true), &Human::pk),
      make_pair(FieldParams("name"), &Human::name),
      make_pair(FieldParams("surname"), &Human::surname),
      make_pair(FieldParams("age"), &Human::age)
  );
  ModelManager::getInstance().registerModel<Dog>(
      "dogs",
      make_pair(FieldParams("pk", true), &Dog::pk),
      make_pair(FieldParams("name"), &Dog::name),
      make_pair(FieldParams("breed"), &Dog::breed),
      make_pair(FieldParams("age"), &Dog::age),
      make_pair(ForeignKeyField("owner", "doggos"),
                &Dog::owner)
  );
  ModelManager::getInstance().registerModel<Cat>(
      "cats",
      make_pair(FieldParams("pk", true), &Cat::pk),
      make_pair(FieldParams("name", false, true, true), &Cat::name),
      make_pair(FieldParams("breed", false, false, false, "breed"),
                &Cat::breed),
      make_pair(FieldParams("age"), &Cat::age)
  );
}

int main() {
  using std::make_pair;

  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  DatabaseManagerFactory<SQLiteDatabaseManager>::createAndConnect(options);

  registerModels();

  Cat c;
  c.age = 50;
  c.save();

//  for (Dog &d : dogs) {
//    std::cout << d.name << std::endl;
//  }

//  Dog d;
//  try {
//    d = Dog::get({{el"name", "Ciccio"}});
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