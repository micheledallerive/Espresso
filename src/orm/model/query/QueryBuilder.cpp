//
// Created by michele on 12.02.23.
//

#include "QueryBuilder.h"
#include <orm/model/ModelManager.h>

namespace Espresso::ORM::Query {

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::filter(const FilterOperation &filter) {
  if (this->filter_.has_value()) {
    this->filter_ = this->filter_.value() & filter;
  } else {
    this->filter_ = filter;
  }
  return *this;
}

template<typename M>
QueryBuilder<M> &QueryBuilder<M>::limit(int limit) {
  this->limit_ = limit;
  return *this;
}

template<typename M>
std::vector<M> QueryBuilder<M>::execute() const {
  const ModelData &data = ModelManager::getInstance().getModel<M>();
  std::string query = "SELECT * FROM " + data.tableName;
  if (this->filter_.has_value()) {
    query += " WHERE " + this->filter_.value().toString();
  }
  if (this->limit_.has_value()) {
    query += " LIMIT " + std::to_string(this->limit_.value());
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
          M::setFieldValue(m, const_cast<ModelDataField&>(fieldData), row.at(fieldName));
        }
        result.push_back(m);
      });
  return result;
}

template<typename M>
QueryBuilder<M>::operator std::vector<M >() const {
  return this->execute();
}

} // ORM