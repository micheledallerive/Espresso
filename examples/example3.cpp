//
// Created by michele on 14.02.23.
//

#include <Server.h>
#include <orm/model/Model.h>
#include <orm/model/ModelManager.h>
#include <orm/model/query/filter/FilterField.h>
#include <orm/database/SQLiteDatabaseManager.h>

#include <iostream>
#include <utility>
#include <thread>

using namespace std;
using namespace Espresso;
using namespace Espresso::ORM;
using namespace Espresso::ORM::Query;

class Test : public Model<Test> {
 public:
  PrimaryKey<int> pk;
  ModelField<std::string> name;
  ModelField<int> count;

  Test() = default;
  Test(std::string n, int c) : name(std::move(n)), count(c) {}
};

void registerModels() {
  ModelManager::getInstance().registerModel<Test>(
      "tests",
      make_pair(FieldParams("pk", true), &Test::pk),
      make_pair(FieldParams("name"), &Test::name),
      make_pair(FieldParams("count"), &Test::count)
  );
}

int main() {
  Espresso::Server server;
  SQLiteConnectionOptions options;
  options.databasePath = "example3.db";
  DatabaseManagerFactory<SQLiteDatabaseManager>::createAndConnect(options);

  registerModels();

  server.router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });

  server.router.get("/create",
                    [](HTTPRequest &request, HTTPResponse &response) {

                      {
                        AtomicTransaction transaction;
                        for (int i = 0; i < 1000; i++) {
                          dbManager->execute(
                              "INSERT INTO tests (name, count) VALUES ('test', 1)");
                        }
                      }
                      response.send("done");
                    });

  server.router.get("/read/:id",
                    [](HTTPRequest &request, HTTPResponse &response) {
                      std::string pk = request.params["id"];
                      Test test = Test::all().filter(Q("pk") == pk).get();
                      response.send("Name: " + (string) test.name + " Count: "
                                        + std::to_string(test.count));
                    });

  server.listen(8888, []() {
    std::cout << "Server started" << std::endl;
  });
  dbManager->disconnect();
  return 0;
}