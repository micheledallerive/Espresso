//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASEMANAGER_H_

#include <string>

namespace Espresso {

class DatabaseManager {
  virtual void connect() = 0;
  virtual void disconnect() = 0;
  virtual void execute(const std::string &query) = 0;
};

}

#endif //ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
