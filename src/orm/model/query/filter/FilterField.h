//
// Created by michele on 11.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTERFIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTERFIELD_H_

#include "FilterNode.h"
#include "FilterOperation.h"

namespace Espresso::ORM::Query {

class FilterField : public FilterNode { // represents a FilterLiteral
 protected:
  std::string value;
 public:
  explicit FilterField(std::string value) : value(std::move(value)) {}
  FilterField(const FilterField &other) : value(other.value) {};
  FilterField(FilterField &&other) : value(std::move(other.value)) {};

  [[nodiscard]] std::string toString() const override;
  [[nodiscard]] std::vector<std::string> getKeys() const override;
  [[nodiscard]] bool isTerminal() const override;

  FilterOperation operator==(const std::string &right) const;
  FilterOperation *operator!=(const std::string &right) const;
  FilterOperation *operator<(const std::string &right) const;
  FilterOperation *operator<=(const std::string &right) const;
  FilterOperation *operator>(const std::string &right) const;
  FilterOperation *operator>=(const std::string &right) const;
  FilterOperation *operator%(const std::string &right) const;

};

using Q = FilterField; // alias for FilterField to make queries more readable

} // Query

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTERFIELD_H_
