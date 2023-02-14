//
// Created by michele on 10.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDPARAMS_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDPARAMS_H_

#include <string>
#include <any>
#include <optional>
#include <utility>
#include "orm/exceptions.h"

namespace Espresso::ORM {

class FieldParams {
 public:
  std::string name{};
  bool autoIncrement{false};
  bool notNull{false};
  std::string defaultValue;
  bool primaryKey{false};

  FieldParams() = default;
  FieldParams(const FieldParams &other) = default;
  FieldParams(FieldParams &&other) noexcept = default;

  explicit FieldParams(std::string name,
              bool autoIncrement = false,
              bool notNull = false,
              std::string defaultValue = "",
              bool primaryKey = false)
      : name(std::move(name)),
        autoIncrement(autoIncrement),
        notNull(notNull),
        defaultValue(std::move(defaultValue)),
        primaryKey(primaryKey) {}

  FieldParams &operator=(const FieldParams &other) = default;
  FieldParams &operator=(FieldParams &&other) noexcept = default;

  virtual void validate() const {
    if (this->name.empty()) {
      throw model_error("Field name cannot be empty");
    }
  }
};

struct ForeignKeyField : public FieldParams {
  std::string relatedName;

  ForeignKeyField() = default;
  ForeignKeyField(const ForeignKeyField &other) = default;
  ForeignKeyField(ForeignKeyField &&other) noexcept = default;

  ForeignKeyField(std::string name,
                  std::string relatedName,
                  bool autoIncrement = false,
                  bool notNull = false,
                  std::string defaultValue = "",
                  bool primaryKey = false)
      : FieldParams(std::move(name),
                    autoIncrement,
                    notNull,
                    std::move(defaultValue),
                    primaryKey),
        relatedName(std::move(relatedName)) {}

  ForeignKeyField(const FieldParams &f, std::string relatedName)
      : FieldParams(f), relatedName(std::move(relatedName)) {}

  void validate() const override {
    FieldParams::validate();
    if (this->relatedName.empty()) {
      throw model_error("Related name cannot be empty");
    }
  }
};

struct ForeignKeyStruct_ {
  std::string table;
  std::string tablePrimaryKey;
  std::string relatedName;
};


} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELDPARAMS_H_
