#include <gtest/gtest.h>
#include <rfl/internal/num_fields.hpp>
#include <orm/reflection/num_fields.hpp>

TEST(Reflection, NoInheritanceNumFields)
{
    struct A {
        int a;
        int b;
        int c;
    };
    struct B{};

    EXPECT_EQ(espresso::orm::refl::num_fields<A>(), 3);
    EXPECT_EQ(espresso::orm::refl::num_fields<B>(), 0);
}

TEST(Reflection, InheritanceNumFields)
{
    struct A {};
    struct A2 {};
    struct B : public A{
        int a;
        int b;
    };
    struct C : public A, public A2 {};

    EXPECT_EQ(espresso::orm::refl::num_fields<B>(), 2);
    EXPECT_EQ(espresso::orm::refl::num_fields<C>(), 0);
}