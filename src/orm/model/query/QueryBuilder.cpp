//
// Created by michele on 12.02.23.
//

#include "QueryBuilder.h"
#include "orm/exceptions.h"
#include <orm/model/ModelManager.h>
#include <sstream>

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
  std::ostringstream sql;
  if (this->filter_.has_value()) {
    sql << " WHERE " << this->filter_.value().toString();
  }
  if (this->order_by_.has_value()) {
    sql << " ORDER BY ";
    for (unsigned int i = 0; i < this->order_by_.value().size(); i++) {
      const auto &order = this->order_by_.value()[i];
      sql << order.first;
      if (!order.second) {
        sql << " DESC";
      }
      if (i < this->order_by_.value().size() - 1) {
        sql << ", ";
      }
    }
  }
  if (this->limit_.has_value()) {
    sql << " LIMIT " << std::to_string(this->limit_.value());
  }
  return sql.str();
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
QueryBuilder<M> &QueryBuilder<M>::reverse() {
  if (this->order_by_.has_value()) {
    for (auto &order : this->order_by_.value()) {
      order.second = !order.second;
    }
  }
  return *this;
}

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::distinct() {
  this->distinct_ = true;
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
M QueryBuilder<M>::get() {
  return *this->get_ptr();
}

template<typename M>
std::shared_ptr<M> QueryBuilder<M>::get_ptr() {
  this->limit(1);
  std::vector<M> result = this->execute();
  if (result.empty()) {
    return nullptr;
  }
  return std::make_shared<M>(result[0]);
}

template<typename M>
template<typename... Args>
M QueryBuilder<M>::create(Args &&... args) {
  M m = M(std::forward<Args>(args)...);
  m.save(false);
  return m;
}

template<typename M>
template<typename... Args>
void QueryBuilder<M>::bulkCreate(std::vector<M> args) {
  for (auto &arg : args) {
    arg.save(false);
  }
}

template<typename M>
std::vector<M> QueryBuilder<M>::execute() {
  if (this->cache_results_.has_value()) {
    return this->cache_results_.value();
  }
  const ModelData &data = ModelManager::getInstance().getModel<M>();

  std::ostringstream sql;
  sql << "SELECT ";
  if (this->distinct_) {
    sql << "DISTINCT ";
  }
  sql << "* FROM " << data.tableName;
  sql << this->conditionsToSQL();
  sql << ";";
  const std::string query = sql.str();
  std::vector<M> result;
  bool found = false;
  dbManager->execute(
      query,
      [&result, &data, &found](
          const std::unordered_map<std::string,
                                   std::string> &row) {
        M m = M();
        if (!row.empty()) {
          found = true;
        }
        for (const auto &field : data.fields) {
          const std::string &fieldName = field.first;
          auto &fieldData =
              const_cast<ModelDataField &>(field.second);
          M::setFieldValue(m, fieldData, row.at(fieldName));
          auto *f = M::getField(m, fieldData);
          f->setDirty(false);
        }
        m.wasSaved = true;
        result.push_back(m);
      });
  if (!found) {
    throw model_error(string("No results found for query: " + query));
  }
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