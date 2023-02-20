//
// Created by michele on 12.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_

#include <orm/model/query/filter/FilterOperation.h>
#include <orm/model/Aggregation.h>
#include <vector>
#include <optional>

namespace Espresso::ORM {
template<typename A>
class Model;
}

namespace Espresso::ORM::Query {

template<typename M> // the model type
class QueryBuilder {
 private:
  using Order = std::pair<std::string, bool>; // field name, ascending
  std::optional<std::vector<M>> cache_results_{std::nullopt};

  std::optional<FilterOperation> filter_{std::nullopt};
  std::optional<int> limit_{std::nullopt};
  std::optional<std::vector<Order>> order_by_{std::nullopt};
  bool distinct_{false};

  // contains the context of the current QueryBuilder
  // when I create new models from this QueryBuilder, I need to know
  // the context that I am creating them in.
  // e.g. the QueryBuilder returned by a related call, must create an element
  // that has the foreign key automatically set to the current model.

  // pair<foreign key field name, foreign key value>
  std::pair<std::string, std::string>
      relationship_context_{std::make_pair("", "")};

  void updated() {
    this->cache_results_ = std::nullopt;
  }
  void checkField(const std::string &) const;
 protected:
  [[nodiscard]] const std::string &getTableName() const;
  [[nodiscard]] const std::string &getPrimaryKey() const;
  [[nodiscard]] std::string conditionsToSQL() const;
 public:
  QueryBuilder() = default;
  virtual ~QueryBuilder() = default;

  // return new QueryBuilder
  QueryBuilder &filter(const FilterOperation &filter);
  QueryBuilder &exclude(const FilterOperation &filter);
  QueryBuilder &order_by(const std::vector<std::string> &);
  QueryBuilder &reverse();
  QueryBuilder &distinct();
  QueryBuilder &limit(int limit);

  // terminating functions
  template<typename... Args>
  M create(Args &&... args);
  void bulkCreate(std::vector<M> args);
  size_t count();
  M get(const std::optional<FilterOperation> & = std::nullopt);
  std::shared_ptr<M> get_ptr(const std::optional<FilterOperation> & = std::nullopt);
  std::optional<M> first();
  std::optional<M> last();
  bool exists();
  bool contains(M &);
  template<typename Fn>
  typename Fn::value_type aggregate(Fn &&fn);

  // dereference functions
  operator std::vector<M>();
  std::vector<M> execute();
 private:
  template<class A>
  friend
  class Espresso::ORM::Model;

  QueryBuilder(const std::string &fkField, const std::string &fkValue)
      : relationship_context_({fkField, fkValue}) {}
};

} // ORM

#include "QueryBuilder.cpp"

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_QUERYBUILDER_H_
