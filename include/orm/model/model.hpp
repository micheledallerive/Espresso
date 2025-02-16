#pragma once
#include "orm/exception.hpp"
#include "orm/queryset/queryset.hpp"
#include "orm/reflection/to_view.hpp"
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
            ((callback(MetaModel<Child>::column_name(refl::get_field_name_str<std::get<_i>(Child::ModelProperties::primary_key)>()),
                       _this()->*std::get<_i>(ptr_pk))),
             ...);
        }(std::make_index_sequence<tuple_size_v<PK>>{});
    }

    template<typename Callback>
    void iterate_db_column_values(Callback&& callback)
    {
        refl::view_apply(*_this(), [&callback]<typename _Field>(const _Field& f) {
            using Field = clean_type_t<typename _Field::Type>;
            const auto col_name = MetaModel<Child>::column_name(std::string(f.name()));
            if constexpr (is_specialization_of_v<Field, ForeignKey>) {
                using OtherModelPtrFK = typename Field::PtrFK;// std::tuple<Type OtherModel::*, Type2 OtherModel::*, ...>
                using OtherModel = typename Field::Model;
                const auto& fk = f.value_ptr()->fk();// values of Tuple<Type, Type2>
                [&]<size_t... _i>(std::index_sequence<_i...>) {
                    ((callback(
                             col_name + "__" + MetaModel<OtherModel>::column_name(refl::get_field_name_str<std::get<_i>(OtherModel::ModelProperties::primary_key)>()),
                             std::get<_i>(fk))),
                     ...);
                }(std::make_index_sequence<tuple_size_v<OtherModelPtrFK>>{});
            }
            else {
                static_assert(!is_specialization_of_v<Field, ForeignKey>);
                callback(col_name, *f.value_ptr());
            }
        });
    }

public:
    auto pk() const
    {
        const auto instance = _this();
        return apply([&instance](auto const &... args) {
            // dereference the pointer to struct field args
            return make_tuple(instance->*args...);
        },
              MetaModel<Child>::compile_time::primary_key());
    }

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

template<typename T>
    requires ModelConcept<T>
using PK = tuple_field_ptr_type_t<decltype(MetaModel<T>::compile_time::primary_key())>;

}// namespace espresso::orm