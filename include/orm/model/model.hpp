#pragma once
#include "orm/exception.hpp"
#include "orm/queryset/queryset.hpp"
#include <stdexcept>

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

    void remove()
    {
        DB::Compiler::Delete del{MetaModel<Child>::compile_time::table_name()};
        const auto view = rfl::to_view(*_this());
        view.apply([&](const auto& f) {
            // if f is a std::optional that is empty, do not add teh filter
            del.filter(MetaModel<Child>::column_name(std::string(f.name())), *f.value());
        });
        const auto query = del.compile();
        DBManager::get().execute_query(query);
    }

    class DoesNotExist : public ObjectDoesNotExist {
    public:
        DoesNotExist() : ObjectDoesNotExist() {}
    };
};

}// namespace espresso::orm