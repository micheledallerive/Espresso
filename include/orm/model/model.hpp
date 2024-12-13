#pragma once
#include "orm/queryset/queryset.hpp"

namespace espresso::orm {

template<typename Child>
class BaseModel {
private:
    Child* _this()
    {
        return static_cast<Child*>(this);
    }

public:
    using ConcreteModel = Child;
    [[maybe_unused]] static QuerySet<Child> objects()
    {
        return {};
    }

    void save()
    {
        DB::Compiler::Insert insert{MetaModel<Child>::compile_time::table_name()};
        const auto view = rfl::to_view(*_this());
        view.apply([&](const auto& f) {
            insert.insert(MetaModel<Child>::column_name(std::string(f.name())), *f.value());
        });
        const auto query = insert.compile();
        DBManager::get().execute_query(query);
    }

    void remove() {
        DB::Compiler::Delete del{MetaModel<Child>::compile_time::table_name()};
        const auto view = rfl::to_view(*_this());
        view.apply([&](const auto& f) {
            del.filter(MetaModel<Child>::column_name(std::string(f.name())), *f.value());
        });
        const auto query = del.compile();
        DBManager::get().execute_query(query);
    }
};

}// namespace espresso::orm