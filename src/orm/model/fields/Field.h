//
// Created by michele on 10.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELD_H_

#include <string>
#include <any>
#include <optional>
#include <utility>
#include "orm/exceptions.h"

namespace Espresso::ORM {

class Field {
 public:
  std::string name{};
  bool autoIncrement{false};
  bool notNull{false};
  std::string defaultValue;
  bool primaryKey{false};

  Field() = default;
  Field(const Field &other) = default;
  Field(Field &&other) noexcept = default;

  Field(std::string name,
        bool autoIncrement = false,
        bool notNull = false,
        std::string defaultValue = "",
        bool primaryKey = false)
      : name(std::move(name)),
        autoIncrement(autoIncrement),
        notNull(notNull),
        defaultValue(std::move(defaultValue)),
        primaryKey(primaryKey) {}

  Field &operator=(const Field &other) = default;
  Field &operator=(Field &&other) noexcept = default;

  virtual void validate() const {
    if (this->name.empty()) {
      throw model_error("Field name cannot be empty");
    }
  }
};

struct ForeignKeyField : public Field {
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
      : Field(std::move(name),
              autoIncrement,
              notNull,
              std::move(defaultValue),
              primaryKey),
        relatedName(std::move(relatedName)) {}

  ForeignKeyField(const Field &f, std::string relatedName)
      : Field(f), relatedName(std::move(relatedName)) {}

  void validate() const override {
    Field::validate();
    if (this->relatedName.empty()) {
      throw model_error("Related name cannot be empty");
    }
  }
};

struct ForeignKeyData {
  std::string table;
  std::string tablePrimaryKey;
  std::string relatedName;
};


} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_FIELD_H_
