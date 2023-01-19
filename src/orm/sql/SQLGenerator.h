//
// Created by michele on 19.01.23.
//

#ifndef ESPRESSO_SRC_ORM_MODELS_SQLGENERATOR_H_
#define ESPRESSO_SRC_ORM_MODELS_SQLGENERATOR_H_

#include <vector>
#include <string>

namespace Espresso {

class SQLGenerator {
 public:
  SQLGenerator() = delete;
  SQLGenerator(const SQLGenerator &) = delete;
  SQLGenerator(SQLGenerator &&) = delete;
  SQLGenerator &operator=(const SQLGenerator &) = delete;
  SQLGenerator &operator=(SQLGenerator &&) = delete;
  static std::string createTable(const std::string &table_name,
                                 const std::vector<std::string> &columns,
                                 const std::vector<std::string> &types,
                                 const std::vector<std::string> &constraints);
  static std::string dropTable(const std::string &table_name);
  static std::string insert(const std::string &table_name,
                            const std::vector<std::string> &columns,
                            const std::vector<std::string> &values);
  static std::string select(const std::string &table_name,
                            const std::vector<std::string> &columns,
                            const std::vector<std::string> &constraints);

};

} // Espresso

#endif //ESPRESSO_SRC_ORM_MODELS_SQLGENERATOR_H_
