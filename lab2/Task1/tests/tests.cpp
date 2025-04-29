#include "BigInt.hpp"
#include <gtest/gtest.h>

    class BigIntTest : public ::testing::Test
{
protected:
    BigInt zero{0};
    BigInt pos123{123};
    BigInt neg456{-456};
    BigInt largePos{"12345678901234567890"};
    BigInt largeNeg{"-98765432109876543210"};
};

TEST_F(BigIntTest, default_constructor)
{
    BigInt num;
    EXPECT_EQ(num, zero);
}

TEST_F(BigIntTest, long_long_constructor)
{
    EXPECT_EQ(BigInt(12345), BigInt("12345"));
    EXPECT_EQ(BigInt(-6789), BigInt("-6789"));
}

TEST_F(BigIntTest, string_constructor)
{
    EXPECT_EQ(BigInt("123"), pos123);
    EXPECT_EQ(BigInt("-456"), neg456);
    EXPECT_THROW(BigInt("12a34"), std::exception);
}

TEST_F(BigIntTest, copy_constructor)
{
    BigInt copy(pos123);
    EXPECT_EQ(copy, pos123);
}

TEST_F(BigIntTest, move_constructor)
{
    BigInt moved(std::move(BigInt(123)));
    EXPECT_EQ(moved, pos123);
}

TEST_F(BigIntTest, copy_assignment)
{
    BigInt copy;
    copy = pos123;
    EXPECT_EQ(copy, pos123);
}

TEST_F(BigIntTest, move_assignment)
{
    BigInt moved;
    moved = std::move(BigInt(123));
    EXPECT_EQ(moved, pos123);
}

TEST_F(BigIntTest, addition_operator)
{
    EXPECT_EQ(pos123 + BigInt(456), BigInt(579));
    EXPECT_EQ(neg456 + BigInt(456), zero);
}

TEST_F(BigIntTest, subtraction_operator)
{
    EXPECT_EQ(pos123 - BigInt(23), BigInt(100));
    EXPECT_EQ(neg456 - BigInt(-400), BigInt(-56));
}

TEST_F(BigIntTest, multiplication_operator)
{
    EXPECT_EQ(pos123 * BigInt(2), BigInt(246));
    EXPECT_EQ(neg456 * BigInt(-1), BigInt(456));
}

TEST_F(BigIntTest, division_operator)
{
    EXPECT_EQ(largePos / BigInt(10), BigInt("1234567890123456789"));
    EXPECT_THROW(pos123 / zero, std::exception);
}

TEST_F(BigIntTest, modulus_operator)
{
    EXPECT_EQ(pos123 % BigInt(100), BigInt(23));
    EXPECT_EQ(neg456 % BigInt(100), BigInt(-56));
}

TEST_F(BigIntTest, left_shift_operator)
{
    EXPECT_EQ(pos123 << BigInt(2), BigInt(492));
    EXPECT_EQ(zero << BigInt(100), BigInt(1));
}

TEST_F(BigIntTest, right_shift_operator)
{
    EXPECT_EQ(BigInt(492) >> BigInt(2), pos123);
    EXPECT_EQ(BigInt(1) >> BigInt(100), zero);
}

TEST_F(BigIntTest, compound_addition)
{
    BigInt num = pos123;
    num += BigInt(377);
    EXPECT_EQ(num, BigInt(500));
}

TEST_F(BigIntTest, compound_subtraction)
{
    BigInt num = pos123;
    num -= BigInt(23);
    EXPECT_EQ(num, BigInt(100));
}

TEST_F(BigIntTest, compound_multiplication)
{
    BigInt num = pos123;
    num *= BigInt(2);
    EXPECT_EQ(num, BigInt(246));
}

TEST_F(BigIntTest, compound_division)
{
    BigInt num = largePos;
    num /= BigInt(10);
    EXPECT_EQ(num, BigInt("1234567890123456789"));
}

TEST_F(BigIntTest, compound_modulus)
{
    BigInt num = pos123;
    num %= BigInt(100);
    EXPECT_EQ(num, BigInt(23));
}

TEST_F(BigIntTest, compound_left_shift)
{
    BigInt num = pos123;
    num <<= BigInt(2);
    EXPECT_EQ(num, BigInt(492));
}

TEST_F(BigIntTest, compound_right_shift)
{
    BigInt num = BigInt(492);
    num >>= BigInt(2);
    EXPECT_EQ(num, pos123);
}

TEST_F(BigIntTest, prefix_increment)
{
    BigInt num = pos123;
    EXPECT_EQ(++num, BigInt(124));
}

TEST_F(BigIntTest, postfix_increment)
{
    BigInt num = pos123;
    EXPECT_EQ(num++, pos123);
    EXPECT_EQ(num, BigInt(124));
}

TEST_F(BigIntTest, prefix_decrement)
{
    BigInt num = pos123;
    EXPECT_EQ(--num, BigInt(122));
}

TEST_F(BigIntTest, postfix_decrement)
{
    BigInt num = pos123;
    EXPECT_EQ(num--, pos123);
    EXPECT_EQ(num, BigInt(122));
}

TEST_F(BigIntTest, equality_operator)
{
    EXPECT_TRUE(pos123 == BigInt(123));
    EXPECT_FALSE(pos123 == neg456);
}

TEST_F(BigIntTest, inequality_operator)
{
    EXPECT_TRUE(pos123 != neg456);
    EXPECT_FALSE(pos123 != BigInt(123));
}

TEST_F(BigIntTest, less_than_operator)
{
    EXPECT_TRUE(neg456 < pos123);
    EXPECT_FALSE(pos123 < neg456);
}

TEST_F(BigIntTest, greater_than_operator)
{
    EXPECT_TRUE(pos123 > neg456);
    EXPECT_FALSE(neg456 > pos123);
}

TEST_F(BigIntTest, less_or_equal_operator)
{
    EXPECT_TRUE(neg456 <= pos123);
    EXPECT_TRUE(pos123 <= BigInt(123));
}

TEST_F(BigIntTest, greater_or_equal_operator)
{
    EXPECT_TRUE(pos123 >= neg456);
    EXPECT_TRUE(pos123 >= BigInt(123));
}

TEST_F(BigIntTest, swap_function)
{
    BigInt a = pos123;
    BigInt b = neg456;
    BigInt::swap(a, b);
    EXPECT_EQ(a, neg456);
    EXPECT_EQ(b, pos123);
}

TEST_F(BigIntTest, length_function)
{
    EXPECT_EQ(largePos.length(), 20);
    EXPECT_EQ(zero.length(), 1);
}

TEST_F(BigIntTest, output_operator)
{
    std::ostringstream oss;
    oss << pos123;
    EXPECT_EQ(oss.str(), "123");
}

TEST_F(BigIntTest, input_operator)
{
    std::istringstream iss("123");
    BigInt num;
    iss >> num;
    EXPECT_EQ(num, pos123);
}

TEST_F(BigIntTest, mod_exp_function)
{
    EXPECT_EQ(mod_exp(BigInt(2), BigInt(10)), BigInt(1024));
    EXPECT_EQ(mod_exp(BigInt(3), BigInt(5), BigInt(7)), BigInt(5));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}