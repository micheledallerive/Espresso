//
// Created by michele on 19.01.23.
//
#include <iostream>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>



int main() {
  Espresso::SQLiteDatabaseManager manager;
  Espresso::SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  manager.connect(options);


  manager.disconnect();
  return 0;
}