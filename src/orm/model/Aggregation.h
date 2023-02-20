//
// Created by michele on 20.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_AGGREGATION_H_
#define ESPRESSO_SRC_ORM_MODEL_AGGREGATION_H_

#include <utility>
#include <sstream>

#include "orm/utils/SQLizable.h"
namespace Espresso::ORM {

class Aggregate : public SQLizable {
 public:
  typedef int value_type;

  Aggregate() = default;
  Aggregate(std::string field, bool distinct)
      : field_(std::move(field)), distinct_(distinct) {}

  [[nodiscard]] std::string toSQL() const override {
    std::stringstream ss;
    ss << function_ << "(";
    if (distinct_) {
      ss << "DISTINCT ";
    }
    ss << field_ << ")";
    return ss.str();
  }
 protected:
  std::string field_;
  bool distinct_;
  std::string function_;
};

class Count : public Aggregate {
 public:
  Count() = default;
  explicit Count(std::string field, bool distinct = false)
      : Aggregate(std::move(field), distinct) {
    function_ = "COUNT";
  }
};

class Sum : public Aggregate {
 public:
  Sum() = default;
  explicit Sum(std::string field, bool distinct = false)
      : Aggregate(std::move(field), distinct) {
    function_ = "SUM";
  }
};

class Avg : public Aggregate {
 public:
  Avg() = default;
  explicit Avg(std::string field, bool distinct = false)
      : Aggregate(std::move(field), distinct) {
    function_ = "AVG";
  }
};

class Min : public Aggregate {
 public:
  Min() = default;
  explicit Min(std::string field, bool distinct = false)
      : Aggregate(std::move(field), distinct) {
    function_ = "MIN";
  }
};

class Max : public Aggregate {
 public:
  Max() = default;
  explicit Max(std::string field, bool distinct = false)
      : Aggregate(std::move(field), distinct) {
    function_ = "MAX";
  }
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_AGGREGATION_H_
