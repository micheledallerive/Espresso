//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASEMANAGER_H_

#include <string>
#include <unordered_map>

#include <functional>
#include <memory>
namespace Espresso::ORM {

using QueryCallback = std::function<void(std::unordered_map<std::string,
                                                            std::string> &)>;

class ConnectionOptions {
 public:
  virtual ~ConnectionOptions() = default;
};

class DatabaseManager {
 public:
  DatabaseManager() = default;
  virtual ~DatabaseManager() = default;
  virtual void connect(const ConnectionOptions &options) = 0;
  virtual void disconnect() = 0;
  virtual void execute(const std::string &query,
                       QueryCallback callback = nullptr) = 0;

  virtual void startTransaction() = 0;
  virtual void commitTransaction() = 0;

  bool isConnected();
 protected:
  bool connected{false};
};

extern std::shared_ptr<DatabaseManager> dbManager;

template<class DM>
class DatabaseManagerFactory {
 public:
  static void create() {
    dbManager = std::make_shared<DM>();
  }
  static void createAndConnect(const ConnectionOptions &options) {
    create();
    dbManager->connect(options);
  }
};

}

#endif //ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
