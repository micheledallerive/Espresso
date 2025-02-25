#include "orm/database/presets/sqlite_instance.hpp"

struct EspressoSettings {
    using DB = espresso::orm::SQLiteInstance;
};

#include "orm/concepts.hpp"
#include "orm/database/db_manager.hpp"
#include "orm/model/field_property.hpp"
#include "orm/model/foreign_key.hpp"
#include "orm/model/meta_model.hpp"
#include "orm/model/model.hpp"
#include "orm/queryset/filter_field.hpp"
#include "orm/queryset/queryset.hpp"
#include "orm/reflection/get_name.hpp"
#include <vector>

using namespace std;
using namespace espresso;
using namespace espresso::orm;

class Dog : public BaseModel<Dog> {
public:
    std::string name;
    int age;

    struct ModelProperties {
        static constexpr auto primary_key = make_tuple(&Dog::name);
    };
};

//static_assert(std::is_same_v<int, decltype(&Dog::age)>);

class User : public BaseModel<User> {
public:
    std::string username;
    int age;
    std::optional<int> agf;
    ForeignKey<Dog> dog;

    struct FieldProperties {
        const FieldPropertyList username{max_length(20), column_name("UserName")};
    };

    struct ModelProperties {
        static constexpr auto primary_key = make_tuple(&User::username, &User::age);
        static constexpr std::string_view table_name = "User";
    };
};
static_assert(ModelConcept<User>);

void print_all()
{
    vector<User> results = User::objects().all();
    for (auto& result : results) {
        cout << result.username << " " << result.age << " ";
        if (result.agf.has_value()) cout << result.agf.value();
        else
            cout << "NULL";
        cout << endl;
        cout << result.dog->name << " " << result.dog->age << endl;
    }
}

int main()
{
    DBManager::get().emplace("test.db");

//    print_all();
//    User::objects().remove();
//    print_all();

    Dog dog{.name = "Fido", .age = 69};
    dog.save();

    User user{.username = "mdr", .age = 120, .dog = dog};
    user.save();

    return 0;
}