#include <orm/database/presets/sqlite_instance.hpp>
#include <string>
#include <utils/tuple.hpp>

struct EspressoSettings {
    using DB = espresso::orm::SQLiteInstance;
};

#include <gtest/gtest.h>
#include <orm/model/model.hpp>

class Model : public espresso::orm::BaseModel<Model> {
public:
    int field1;
    int second_field;

    struct ModelProperties {
        static constexpr auto primary_key = espresso::make_tuple(&Model::field1);
    };
};

class Model2 : public espresso::orm::BaseModel<Model2> {
public:
    int field1;
    std::string second_field;

    struct ModelProperties {
        static constexpr auto primary_key = espresso::make_tuple(&Model2::field1, &Model2::second_field);
    };
};

TEST(ORM, SinglePrimaryKey)
{
    Model instance{.field1 = 5, .second_field = -151};

    std::tuple<int> expected{5};
    EXPECT_EQ(expected, instance.pk());
}

TEST(ORM, MultiPrimaryKey)
{
    Model2 instance{.field1 = 5, .second_field = "-151"};
    std::tuple<int, std::string> expected{5, "-151"};
    EXPECT_EQ(expected, instance.pk());
}