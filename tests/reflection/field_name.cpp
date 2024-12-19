#include <orm/reflection/get_name.hpp>
#include <gtest/gtest.h>

struct MyStruct {
    int first_field;
    double field_2;
    char third_field_in_a_row;
};

using namespace espresso::orm::refl;

TEST(Reflection, FieldName) {
    EXPECT_EQ(get_field_name_str<&::MyStruct::first_field>(), "first_field");
    EXPECT_EQ(get_field_name_str<&::MyStruct::field_2>(), "field_2");
    EXPECT_EQ(get_field_name_str<&::MyStruct::third_field_in_a_row>(), "third_field_in_a_row");
}