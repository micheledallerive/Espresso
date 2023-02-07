//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASEMANAGER_H_

#include <string>

#include <functional>
#include <memory>
namespace Espresso::ORM {

using QueryCallback = std::function<void(std::vector<std::pair<std::string,
                                                               std::string>>&)>;

class ConnectionOptions {
 public:
  virtual ~ConnectionOptions() = default;
};

class DatabaseManager {
 public:
  DatabaseManager() = default;
  virtual void connect(const ConnectionOptions &options) = 0;
  virtual void disconnect() = 0;
  virtual void execute(const std::string &query,
                       QueryCallback callback) = 0;

};

extern std::shared_ptr<DatabaseManager> dbManager;

}

#endif //ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
