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
  std::optional<FilterOperation> filter_{std::nullopt};
  std::optional<int> limit_{std::nullopt};
 public:
  QueryBuilder() = default;
  virtual ~QueryBuilder() = default;

  QueryBuilder &filter(const FilterOperation &filter);
  QueryBuilder &limit(int limit);

  operator std::vector<M>() const;
  std::vector<M> execute() const;
};

} // ORM

#include "QueryBuilder.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_
