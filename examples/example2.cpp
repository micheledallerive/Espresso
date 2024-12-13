#include "orm/database/presets/sqlite_instance.hpp"

struct EspressoSettings {
    using DB = espresso::orm::SQLiteInstance;
};

#include "orm/concepts.hpp"
#include "orm/database/db_manager.hpp"
#include "orm/model/field_property.hpp"
#include "orm/model/meta_model.hpp"
#include "orm/model/model.hpp"
#include "orm/queryset/filter_field.hpp"
#include "orm/queryset/queryset.hpp"
#include "orm/reflection/field_name.hpp"
#include "rfl/field_names_t.hpp"
#include <rfl/fields.hpp>
#include <rfl/get.hpp>
#include <vector>

using namespace std;
using namespace espresso;
using namespace espresso::orm;

class User : public BaseModel<User> {
public:
    std::string username;
    int age;
    std::optional<int> agf;

    struct FieldProperties {
        const FieldPropertyList username{max_length(20), column_name("UserName")};
    };

    struct ModelProperties {
        static constexpr auto primary_key = std::make_tuple(&User::username, &User::age);
        static constexpr std::string_view table_name = "User";
    };
};

static_assert(std::is_aggregate_v<User>);
static_assert(rfl::internal::num_fields<User> == 3);

int main()
{
    DBManager::get().emplace("test.db");

    auto meta = MetaModel<User>::instance();

    const vector<User> results = User::objects().all();
    for (const auto& result : results) {
        cout << result.username << " " << result.age << " ";
        if (result.agf.has_value()) cout << result.agf.value();
        else
            cout << "NULL";
        cout << endl;
    }

    try {
        auto result = User::objects().filter(field_t<&User::username> == "Lore").get();
        cout << result.username << endl;
        result.remove();
    }
    catch (const User::DoesNotExist& e) {
        cout << "User does not exist!" << endl;
    }

    User u{.username = "Lore", .age = 22, .agf = std::nullopt};
//    u.save();
    //    u.remove();
    //    u.save();

    return 0;
}