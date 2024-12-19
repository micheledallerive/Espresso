#pragma once

#include "orm/database/db_manager.hpp"
#include "orm/queryset/filter_field.hpp"
#include "orm/queryset/queryset.hpp"
#include "orm/utils/types.hpp"
#include "utils/tuple.hpp"
#include <optional>

namespace espresso::orm {

template<typename ToModel>
class ForeignKey {
public:
    using PtrFK = std::remove_cvref_t<decltype(ToModel::ModelProperties::primary_key)>;
    using FK = tuple_field_ptr_type_t<PtrFK>;
    using Model = ToModel;
    using Compound = FK;

    static_assert(is_specialization_of_v<PtrFK, std::tuple>);
    static_assert(is_specialization_of_v<FK, std::tuple>);

private:
    std::variant<ToModel, FK> m_data;

    bool has_model_saved()
    {
        return m_data.index() == 0;
    }

public:
    void retrieve();

    ForeignKey() = default;
    explicit ForeignKey(const FK& fk) : m_data(fk) {}
    ForeignKey(const ToModel& model) : m_data(model) {}

    ToModel& operator*()
    {
        retrieve();
        return std::get<0>(m_data);
    }
    const ToModel& operator*() const
    {
        retrieve();
        return std::get<0>(m_data);
    }
    ToModel* operator->()
    {
        retrieve();
        return &std::get<0>(m_data);
    }
    const ToModel* operator->() const
    {
        retrieve();
        return &std::get<0>(m_data);
    }

    ForeignKey<ToModel>& operator=(const ToModel& obj)
    {
        m_data = obj;
        return *this;
    }

    // operator to cast ToModel to ForeignKey<ToModel>

};

template<typename ToModel>
void ForeignKey<ToModel>::retrieve()
{
    if (has_model_saved()) return;

    QuerySet<ToModel> qs;
    [&]<size_t... _i>(std::index_sequence<_i...>) {
        (qs.filter(field_t<std::get<_i>(ToModel::ModelProperties::primary_key)> == std::get<_i>(std::get<FK>(m_data))), ...);
    }(std::make_index_sequence<tuple_size_v<FK>>{});
    m_data.template emplace<0>(qs.get());
}

}// namespace espresso::orm