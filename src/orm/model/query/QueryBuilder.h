//
// Created by michele on 12.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_

#include <orm/model/query/filter/FilterOperation.h>
#include <vector>

namespace Espresso::ORM::Query {

template<typename M> // the model type
class QueryBuilder {
 private:
  std::optional<std::vector<M>> cache_results_{std::nullopt};

  std::optional<FilterOperation> filter_{std::nullopt};
  std::optional<int> limit_{std::nullopt};

  void updated() {
    this->cache_results_ = std::nullopt;
  }
 protected:
  [[nodiscard]] const std::string &getTableName() const;
  [[nodiscard]] std::string conditionsToSQL() const;
 public:
  QueryBuilder() = default;
  virtual ~QueryBuilder() = default;

  QueryBuilder &filter(const FilterOperation &filter);
  QueryBuilder &exclude(const FilterOperation &filter);
  QueryBuilder &limit(int limit);

  template<typename... Args>
  M create(Args &&... args);
  size_t count();
  M get();
  M *get_ptr();

  operator std::vector<M>();
  std::vector<M> execute();
};

} // ORM

#include "QueryBuilder.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_
