#include <gtest/gtest.h>
#include <orm/reflection/to_view.hpp>
#include <orm/reflection/get_name.hpp>

#include <string>

TEST(Reflection, ToTuple)
{
    struct A {
        int field_1;
        int field_2;
    };

    using namespace espresso::orm::refl;
    using namespace std;

    const A a{.field_1 = 5, .field_2 = 42};

    int idx = 0;
    view_apply(a, [&idx]<typename Field>(const Field& field) {
        if (idx == 0) {
            EXPECT_EQ(field.name(), "field_1");
            auto val = *field.value_ptr();
            EXPECT_EQ(val, 5);
        }
        else {
            EXPECT_EQ(field.name(), "field_2");
            auto val = *field.value_ptr();
            EXPECT_EQ(val, 42);
        }
        ++idx;
    });
}