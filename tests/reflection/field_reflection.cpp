#include <orm/reflection/fields.hpp>
#include <orm/reflection/get_name.hpp>
#include <gtest/gtest.h>

TEST(Reflection, CTField) {
    using namespace espresso::orm::refl;
    struct A {
        int test;
        int hello;
    };

    const auto str = std::string(internal::get_field_name<A, nth_field<A, 0>()>());
    EXPECT_EQ(str, "test");
}