//
// Created by michele on 12.02.23.
//

#include <orm/utils/PrimitiveWrapper.h>
#include <gtest/gtest.h>

using namespace Espresso::ORM;

namespace {

// Test fixture for PrimitiveWrapper<int>
class PrimitiveWrapperIntTest : public ::testing::Test {
 public:
  friend class PrimitiveWrapper<int>;
 protected:
  PrimitiveWrapper<int> wrapper;
};

// Tests the default constructor of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, DefaultConstructor) {
}

// Tests the value constructor of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, ValueConstructor) {
  PrimitiveWrapper<int> wrapper(10);
  EXPECT_EQ(10, wrapper.get());
}

// Tests the copy constructor of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, CopyConstructor) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(wrapper);
  EXPECT_EQ(10, other_wrapper.get());
}

// Tests the move constructor of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, MoveConstructor) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(std::move(wrapper));
  EXPECT_EQ(10, other_wrapper.get());
}

// Tests the value assignment operator of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, ValueAssignmentOperator) {
  PrimitiveWrapper<int> wrapper;
  wrapper = 10;
  EXPECT_EQ(10, wrapper.get());
}

// Tests the assignment operator of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, AssignmentOperator) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper;
  other_wrapper = wrapper;
  EXPECT_EQ(10, other_wrapper.get());
}

// Tests the move assignment operator of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, MoveAssignmentOperator) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper;
  other_wrapper = std::move(wrapper);
  EXPECT_EQ(10, other_wrapper.get());
}

// Tests the conversion operator of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, ConversionOperator) {
  PrimitiveWrapper<int> wrapper(10);
  int value = wrapper;
  EXPECT_EQ(10, value);
}

// Tests the get method of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, Get) {
  PrimitiveWrapper<int> wrapper(10);
  EXPECT_EQ(10, wrapper.get());
}

// Tests the arithmetic operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, ArithmeticOperators) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(5);
  EXPECT_EQ(15, (wrapper + other_wrapper).get());
  EXPECT_EQ(5, (wrapper - other_wrapper).get());
  EXPECT_EQ(50, (wrapper * other_wrapper).get());
  EXPECT_EQ(2, (wrapper / other_wrapper).get());
  EXPECT_EQ(0, (wrapper % other_wrapper).get());

  EXPECT_EQ(20, wrapper + 10);
  EXPECT_EQ(0, wrapper - 10);
  EXPECT_EQ(100, wrapper * 10);
  EXPECT_EQ(2, wrapper / 5);
  EXPECT_EQ(0, wrapper % 5);
}

// Tests the comparison operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, ComparisonOperators) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(5);
  EXPECT_TRUE(wrapper > other_wrapper);
  EXPECT_TRUE(wrapper >= other_wrapper);
  EXPECT_FALSE(wrapper < other_wrapper);
  EXPECT_FALSE(wrapper <= other_wrapper);
  EXPECT_FALSE(wrapper == other_wrapper);
  EXPECT_TRUE(wrapper != other_wrapper);

  EXPECT_TRUE(wrapper > 5);
  EXPECT_TRUE(wrapper >= 5);
  EXPECT_FALSE(wrapper < 5);
  EXPECT_FALSE(wrapper <= 5);
  EXPECT_FALSE(wrapper == 5);
  EXPECT_TRUE(wrapper != 5);
}

// Tests the increment and decrement operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, IncrementDecrementOperators) {
  PrimitiveWrapper<int> wrapper(10);
  EXPECT_EQ(11, (++wrapper).get());
  EXPECT_EQ(11, (wrapper++).get());
  EXPECT_EQ(12, wrapper.get());
  EXPECT_EQ(11, (--wrapper).get());
  EXPECT_EQ(11, (wrapper--).get());
  EXPECT_EQ(10, wrapper.get());
}

// Tests the bitwise operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, BitwiseOperators) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(5);
  EXPECT_EQ(15, (wrapper | other_wrapper).get());
  EXPECT_EQ(0, (wrapper & other_wrapper).get());
  EXPECT_EQ(15, (wrapper ^ other_wrapper).get());
  EXPECT_EQ(-11, (~wrapper).get());
  EXPECT_EQ(40, (wrapper << 2).get());
  EXPECT_EQ(2, (wrapper >> 2).get());
  EXPECT_EQ(320, (wrapper << other_wrapper).get());
  EXPECT_EQ(0, (wrapper >> other_wrapper).get());

  EXPECT_EQ(15, wrapper | 5);
  EXPECT_EQ(0, wrapper & 5);
  EXPECT_EQ(15, wrapper ^ 5);
  EXPECT_EQ(-11, (~wrapper));
  EXPECT_EQ(40, wrapper << 2);
  EXPECT_EQ(2, wrapper >> 2);
}

// Tests the logical operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, LogicalOperators) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(5);
  EXPECT_TRUE(wrapper && other_wrapper);
  EXPECT_TRUE(wrapper || other_wrapper);
  EXPECT_FALSE(!wrapper);

  EXPECT_TRUE(wrapper && 5);
  EXPECT_TRUE(wrapper || 5);
  EXPECT_FALSE(!wrapper);
}

// Tests the assignment operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, AssignmentOperators) {
  PrimitiveWrapper<int> wrapper(10);
  PrimitiveWrapper<int> other_wrapper(5);
  wrapper += other_wrapper;
  EXPECT_EQ(15, wrapper.get());
  wrapper -= other_wrapper;
  EXPECT_EQ(10, wrapper.get());
  wrapper *= other_wrapper;
  EXPECT_EQ(50, wrapper.get());
  wrapper /= other_wrapper;
  EXPECT_EQ(10, wrapper.get());
  wrapper %= other_wrapper;
  EXPECT_EQ(0, wrapper.get());

  wrapper += 5;
  EXPECT_EQ(5, wrapper.get());
  wrapper -= 5;
  EXPECT_EQ(0, wrapper.get());
  wrapper *= 5;
  EXPECT_EQ(0, wrapper.get());
  wrapper /= 5;
  EXPECT_EQ(0, wrapper.get());
  wrapper %= 5;
  EXPECT_EQ(0, wrapper.get());

  wrapper |= other_wrapper;
  EXPECT_EQ(5, wrapper.get());
  wrapper &= other_wrapper;
  EXPECT_EQ(5, wrapper.get());
  wrapper ^= other_wrapper;
  EXPECT_EQ(0, wrapper.get());
  wrapper <<= 2;
  EXPECT_EQ(0, wrapper.get());
  wrapper >>= 2;
  EXPECT_EQ(0, wrapper.get());
}

// Tests the streams operators of PrimitiveWrapper<int>
TEST_F(PrimitiveWrapperIntTest, StreamOperators) {
  PrimitiveWrapper<int> wrapper(10);
  std::stringstream stream;
  stream << wrapper;
  EXPECT_EQ("10", stream.str());
  PrimitiveWrapper<int> otherWrapper(5);
  stream >> otherWrapper;
  EXPECT_EQ(10, otherWrapper.get());
}

}
