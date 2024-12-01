#include "orm/database/presets/sqlite_instance.hpp"
#include "orm/model/field_property.hpp"
#include "orm/model/validate.hpp"
#include <rfl/fields.hpp>
#include <vector>

using namespace std;
using namespace espresso;
using namespace espresso::orm;

struct User {
    std::string username;
    int age;

    struct FieldProperties {
        const FieldPropertyList username{max_length(20), required()};
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

void fn(std::tuple<int, int> row)
{
}

int main()
{
    //static_assert(ModelConcept<User>);

    auto db = SQLiteInstance("test.db");

    db.execute_query("CREATE TABLE IF NOT EXISTS test (username TEXT, age INTEGER);");

    //db.execute_query("SELECT * FROM test;", fn);
    db.execute_query("SELECT * FROM test;", [](std::tuple<std::string, int> row) {
        cout << std::get<0>(row) << " " << std::get<1>(row) << endl;
    });

    return 0;
}