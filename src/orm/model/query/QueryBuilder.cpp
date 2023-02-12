//
// Created by michele on 12.02.23.
//

#include "QueryBuilder.h"
#include <orm/model/ModelManager.h>

namespace Espresso::ORM::Query {

template<typename M>
const std::string &QueryBuilder<M>::getTableName() const {
  return ModelManager::getInstance().getModel<M>().tableName;
}
template<typename M>
void QueryBuilder<M>::checkField(const std::string &field) const {
  if (!ModelManager::getInstance().getModel<M>().fields.contains(field)) {
    throw std::invalid_argument("Field " + field + " does not exist in model " +
        ModelManager::getInstance().getModel<M>().tableName);
  }
}
template<typename M>
std::string QueryBuilder<M>::conditionsToSQL() const {
  std::string sql = "";
  if (this->filter_.has_value()) {
    sql += this->filter_.value().toString();
  }
  if (this->limit_.has_value()) {
    sql += "LIMIT " + std::to_string(this->limit_.value());
  }
  return sql.empty() ? "" : " WHERE " + sql;
}

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::filter(const FilterOperation &filter) {
  if (this->filter_.has_value()) {
    this->filter_ = this->filter_.value() & filter;
  } else {
    this->filter_ = filter;
  }

  this->updated();
  return *this;
}

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::exclude(const FilterOperation &filter) {
  if (this->filter_.has_value()) {
    this->filter_ = this->filter_.value() & (!filter);
  } else {
    this->filter_ = (!filter);
  }

  this->updated();
  return *this;
}

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::order_by(const std::vector<std::string> &order) {
  for (const auto &fieldOrder : order) {
    std::string fieldName;
    bool ascending = fieldOrder[0] != '-';
    if (fieldOrder[0] == '-') {
      fieldName = fieldOrder.substr(1);
      ascending = false;
    } else {
      fieldName = fieldOrder;
    }
    this->checkField(fieldName);
    if (this->order_by_.has_value()) {
      this->order_by_.value().emplace_back(fieldName, ascending);
    } else {
      this->order_by_ = {{fieldName, ascending}};
    }
  }
  return *this;
}

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::limit(int limit) {
  this->limit_ = limit;

  this->updated();
  return *this;
}

template<typename M>
size_t QueryBuilder<M>::count() {
  if (this->cache_results_.has_value()) {
    return this->cache_results_.value().size();
  }
  std::string query = "SELECT COUNT(*) FROM " + this->getTableName();
  query += this->conditionsToSQL();
  query += ";";
  size_t result = 0;
  dbManager->execute(
      query,
      [&result](
          const std::unordered_map<std::string,
                                   std::string> &row) {
        result = std::stoi(row.at("COUNT(*)"));
      });
  return result;
}

template<typename M>
template<typename... Args>
M QueryBuilder<M>::create(Args &&... args) {
  M m = M(std::forward<Args>(args)...);
  m.save(false);
  return m;
}

template<typename M>
std::vector<M> QueryBuilder<M>::execute() {
  if (this->cache_results_.has_value()) {
    return this->cache_results_.value();
  }
  const ModelData &data = ModelManager::getInstance().getModel<M>();
  std::string query = "SELECT * FROM " + data.tableName;
  if (this->filter_.has_value()) {
    query += " WHERE " + this->filter_.value().toString();
  }
  if (this->limit_.has_value()) {
    query += " LIMIT " + std::to_string(this->limit_.value());
  }
  if (this->order_by_.has_value()) {
    query += " ORDER BY ";
    for (const auto &fieldOrder : this->order_by_.value()) {
      query += fieldOrder.first + (fieldOrder.second ? " ASC" : " DESC") + ", ";
    }
    query.pop_back();
    query.pop_back();
  }
  query += ";";
  std::vector<M> result;
  dbManager->execute(
      query,
      [&result, &data](
          const std::unordered_map<std::string,
                                   std::string> &row) {
        M m = M();
        for (const auto &field : data.fields) {
          const std::string &fieldName = field.first;
          const ModelDataField &fieldData = field.second;
          M::setFieldValue(m,
                           const_cast<ModelDataField &>(fieldData),
                           row.at(fieldName));
        }
        result.push_back(m);
      });
  this->cache_results_ = result;
  // q: why does the previous line giving me a operator= error?
  // a: because the cache_results_ is a const std::optional
  // q: how do i fix that
  // a: you can use the mutable keyword
  return result;
}

template<typename M>
QueryBuilder<M>::operator std::vector<M>() {
  return this->execute();
}

} // ORM