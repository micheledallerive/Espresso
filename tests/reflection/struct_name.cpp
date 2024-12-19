#include <orm/reflection/get_name.hpp>
#include <gtest/gtest.h>

struct MyStruct {};
struct OtherMyStruct {};
struct Struct123Struct {};

using namespace espresso::orm::refl;

TEST(Reflection, FieldName) {
    EXPECT_EQ(get_type_name_str<::MyStruct>(), "MyStruct");
    EXPECT_EQ(get_type_name_str<::OtherMyStruct>(), "OtherMyStruct");
    EXPECT_EQ(get_type_name_str<::Struct123Struct>(), "Struct123Struct");
}
