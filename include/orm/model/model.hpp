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
    const Child* _this() const
    {
        return static_cast<const Child*>(this);
    }

    template<typename Callback>
    void iterate_pk_db_column_values(Callback&& callback)
    {
        using PtrPK = decltype(Child::ModelProperties::primary_key);// std::tuple<Type Child::*, Type2 Child::*, ...>
        using PK = tuple_field_ptr_type_t<PtrPK>;                   // std::tuple<Type, Type2, ...>
        constexpr auto ptr_pk = Child::ModelProperties::primary_key;
        [&]<size_t... _i>(std::index_sequence<_i...>) {
            ((callback(MetaModel<Child>::column_name(get_field_name_str<std::get<_i>(Child::ModelProperties::primary_key)>()),
                       _this()->*std::get<_i>(ptr_pk))),
             ...);
        }(std::make_index_sequence<rfl::tuple_size_v<PK>>{});
    }

    template<typename Callback>
    void iterate_db_column_values(Callback&& callback)
    {
        const auto view = rfl::to_view(*_this());
        view.apply([&callback]<typename _Field>(const _Field& f) {
            using Field = std::remove_cvref_t<std::remove_pointer_t<typename _Field::Type>>;
            const auto col_name = MetaModel<Child>::column_name(std::string(f.name()));
            if constexpr (is_specialization_of_v<Field, ForeignKey>) {
                using OtherModelPtrFK = typename Field::PtrFK;// std::tuple<Type OtherModel::*, Type2 OtherModel::*, ...>
                using OtherModel = typename Field::Model;
                const auto& fk = f.value()->fk();// values of Tuple<Type, Type2>
                [&]<size_t... _i>(std::index_sequence<_i...>) {
                    ((callback(
                             col_name + "_" + MetaModel<OtherModel>::column_name(get_field_name_str<std::get<_i>(OtherModel::ModelProperties::primary_key)>()),
                             std::get<_i>(fk))),
                     ...);
                }(std::make_index_sequence<rfl::tuple_size_v<OtherModelPtrFK>>{});
            }
            else {
                static_assert(!is_specialization_of_v<Field, ForeignKey>);
                callback(col_name, *f.value());
            }
        });
    }

public:
    [[maybe_unused]] static QuerySet<Child> objects()
    {
        return {};
    }

    void save()
    {
        DB::Compiler::Insert insert{MetaModel<Child>::compile_time::table_name()};
        iterate_db_column_values([&](const auto& name, const auto& value) {
            insert.insert(name, value);
        });
        const auto query = insert.compile();
        DBManager::get().execute_query(query);
    }

    void remove()
    {
        DB::Compiler::Delete del{MetaModel<Child>::compile_time::table_name()};
        const auto view = rfl::to_view(*_this());
        iterate_pk_db_column_values([&](const auto& name, const auto& value) {
            del.filter(name, value);
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