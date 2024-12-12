#pragma once

#include "orm/utils.hpp"
#include "orm/database/db_manager.hpp"
#include "orm/database/sql_compiler.hpp"
#include "orm/model/meta_model.hpp"
#include "orm/queryset/filter.hpp"

namespace espresso::orm {

template<typename Model>
class QuerySet {
private:
    SQLCompiler m_compiler;

    template<typename Callback>
    void execute(Callback &&callback) const {
        DBManager::get().execute_query(m_compiler.compile(), std::forward<Callback>(callback));
    }

public:
    QuerySet() : m_compiler(MetaModel<Model>::compile_time::table_name()) {}
    ~QuerySet() = default;

    QuerySet& filter(const Filter<Model>& filter)
    {
        m_compiler.filter(filter.str());
        return *this;
    }

    std::vector<Model> all() const
    {
        std::vector<Model> result;
        using Tuple = model_to_tuple_t<Model>;
        execute([&](const Tuple &tuple) {
            result.emplace_back(construct_from_tuple<Model>(tuple));
        });
        return result;
    }
};

}// namespace espresso::orm