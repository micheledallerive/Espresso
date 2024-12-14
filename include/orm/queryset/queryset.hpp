#pragma once

#include "orm/database/db_manager.hpp"
#include "orm/model/meta_model.hpp"
#include "orm/queryset/filter.hpp"
#include "utils/maybe_uninit.hpp"

namespace espresso::orm {

template<typename Model>
class QuerySet {
private:
    DB::Compiler::Query m_compiler;

    template<typename Callback>
    size_t execute(Callback&& callback) const
    {
        return DBManager::get().execute_query(m_compiler.compile(), std::forward<Callback>(callback));
    }

public:
    QuerySet() : m_compiler(MetaModel<Model>::compile_time::table_name()) {}
    ~QuerySet() = default;

    QuerySet& filter(const Filter& filter)
    {
        m_compiler.filter(filter.str());
        return *this;
    }

    Model get() const
    {
        MaybeUninit<Model> result;
        auto cnt = execute([&](const model_to_tuple_t<Model>& tuple) {
            result.emplace(construct_from_tuple<Model>(tuple));
        });
        if (cnt == 0) {
            throw typename Model::DoesNotExist{};
        }
        return result.get();
    }

    std::vector<Model> all() const
    {
        std::vector<Model> result;
        using Tuple = model_to_tuple_t<Model>;
        execute([&](const Tuple& tuple) {
            result.emplace_back(construct_from_tuple<Model>(tuple));
        });
        return result;
    }
};

}// namespace espresso::orm