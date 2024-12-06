#include "orm/database/presets/sqlite_instance.hpp"
#include "orm/model/field_property.hpp"
#include "orm/model/validate.hpp"
#include "orm/queryset/queryset.hpp"
#include "orm/reflection/field_name.hpp"
#include "rfl/field_names_t.hpp"
#include <rfl/fields.hpp>
#include <rfl/get.hpp>
#include <vector>

using namespace std;
using namespace espresso;
using namespace espresso::orm;

struct User {
    std::string username;
    int age;
    int agf;

    int test() {
        return 2;
    }

    struct FieldProperties {
        const FieldPropertyList username{max_length(20), required(), column_name("UserName")};
        const FieldPropertyList age{required()};
    };

    struct ModelProperties {
        static constexpr auto primary_key = std::make_tuple(&User::username, &User::age);
        static constexpr std::string_view table_name = "test";
    };
};

//
//static_assert(rfl::field_names_t<User>::size() == 2);
//static_assert(all_equal<PropertiesList, User::FieldProperties>());

//static_assert(std::is_same_v<decltype(&User::age), int User::*>);
//
//static_assert(std::is_same_v<int,
//                             std::remove_cvref_t<
//                                     std::remove_pointer_t<
//                                             typename std::remove_pointer_t<
//                                                     decltype(rfl::internal::get_ith_field_from_fake_object<User, 1>())>>>>);
//static_assert(rfl::internal::get_field_name_str_view<User, &User::age>()
//              == rfl::internal::get_field_name_str_view<User, &User::agf>());
//static_assert(std::is_same_v<decltype(rfl::internal::get_ith_field_from_fake_object<User, 1>()),
//                             decltype(rfl::internal::get_ith_field_from_fake_object<User, 2>())>);

int main()
{
    //    static_assert(ModelConcept<User>);
    auto db = SQLiteInstance("test.db");

    db.execute_query("CREATE TABLE IF NOT EXISTS test (username TEXT, age INTEGER);");

    //db.execute_query("SELECT * FROM test;", fn);
    db.execute_query("SELECT * FROM test;", [](std::tuple<std::string, int> row) {
        cout << std::get<0>(row) << " " << std::get<1>(row) << endl;
    });

    QuerySet<User> qs;
    qs.filter(field<&User::age> == 3);
    qs.filter(field<&User::age> == field<&User::agf>);

    return 0;
}