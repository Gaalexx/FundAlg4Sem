#include "BigInt.hpp"
#include <gtest/gtest.h>

class BigIntTest : public ::testing::Test
{
protected:
    void SetUp() override {
        bigNum1 = BigInt("1234567890123456789012345678901234567890");
        bigNum2 = BigInt("9876543210987654321098765432109876543210");
        bigNum3 = BigInt("9999999999999999999999999999999999999999");
        bigNum4 = BigInt("10000000000000000000000000000000000000000");
        bigNum5 = BigInt("123456789012345678901234567890123456789012345678901234567890");
        bigNum6 = BigInt("987654321098765432109876543210987654321098765432109876543210");
        zero = BigInt(0);
        pos123 = BigInt(123);
        neg456 = BigInt(-456);
        largePos = BigInt("12345678901234567890");
        largeNeg = BigInt("-98765432109876543210");
    }

    BigInt bigNum1;
    BigInt bigNum2;
    BigInt bigNum3;
    BigInt bigNum4;
    BigInt bigNum5;
    BigInt bigNum6;
    BigInt zero;
    BigInt pos123;
    BigInt neg456;
    BigInt largePos;
    BigInt largeNeg;
};

TEST_F(BigIntTest, DefaultConstructorCreatesZero) {
    BigInt num(0);
    EXPECT_EQ(num, zero);
}

TEST_F(BigIntTest, LongLongConstructorHandlesPositiveNumbers) {
    EXPECT_EQ(BigInt(12345), BigInt("12345"));
}

TEST_F(BigIntTest, LongLongConstructorHandlesNegativeNumbers) {
    EXPECT_EQ(BigInt(-6789), BigInt("-6789"));
}

TEST_F(BigIntTest, StringConstructorHandlesValidPositiveNumber) {
    EXPECT_EQ(BigInt("123"), pos123);
}

TEST_F(BigIntTest, StringConstructorHandlesValidNegativeNumber) {
    EXPECT_EQ(BigInt("-456"), neg456);
}

TEST_F(BigIntTest, StringConstructorThrowsOnInvalidCharacters) {
    EXPECT_THROW(BigInt("12a34"), std::exception);
}

TEST_F(BigIntTest, CopyConstructorCreatesEqualObject) {
    BigInt copy(pos123);
    EXPECT_EQ(copy, pos123);
}

TEST_F(BigIntTest, CopyAssignmentCreatesEqualObject) {
    BigInt copy;
    copy = pos123;
    EXPECT_EQ(copy, pos123);
}

TEST_F(BigIntTest, AdditionOfPositiveNumbersWorks) {
    EXPECT_EQ(pos123 + BigInt(456), BigInt(579));
}

TEST_F(BigIntTest, AdditionWithNegativeNumberWorks) {
    EXPECT_EQ(neg456 + BigInt(456), zero);
}

TEST_F(BigIntTest, AdditionWithVeryLargeNumbersWorks) {
    EXPECT_EQ(BigInt(110000010000) + BigInt("-1100011111110001123400000000000000000000"), 
              BigInt("-1100011111110001123399999999889999990000"));
}

TEST_F(BigIntTest, AdditionWithCarryWorks) {
    EXPECT_EQ(BigInt(999999999999999) + BigInt(1), BigInt(1000000000000000));
}

TEST_F(BigIntTest, SubtractionOfPositiveNumbersWorks) {
    EXPECT_EQ(pos123 - BigInt(23), BigInt(100));
}

TEST_F(BigIntTest, SubtractionWithNegativeNumbersWorks) {
    EXPECT_EQ(neg456 - BigInt(-400), BigInt(-56));
}

TEST_F(BigIntTest, SubtractionWithVeryLargeNumbersWorks) {
    EXPECT_EQ(BigInt(110000010000) - BigInt("1100011111110001123400000000000000000000"), 
              BigInt("-1100011111110001123399999999889999990000"));
}

TEST_F(BigIntTest, MultiplicationOfPositiveNumbersWorks) {
    EXPECT_EQ(pos123 * BigInt(2), BigInt(246));
}

TEST_F(BigIntTest, MultiplicationWithNegativeNumbersWorks) {
    EXPECT_EQ(neg456 * BigInt(-1), BigInt(456));
}

TEST_F(BigIntTest, DivisionOfLargeNumberWorks) {
    EXPECT_EQ(largePos / BigInt(10), BigInt("1234567890123456789"));
}

TEST_F(BigIntTest, DivisionByZeroThrowsException) {
    EXPECT_THROW(pos123 / zero, std::exception);
}

TEST_F(BigIntTest, ModulusOperationWorks) {
    EXPECT_EQ(pos123 % BigInt(100), BigInt(23));
}

TEST_F(BigIntTest, ModulusWithNegativeNumbersWorks) {
    EXPECT_EQ(neg456 % BigInt(100), BigInt(-56));
}

TEST_F(BigIntTest, VeryLargeAdditionWorks) {
    BigInt result = bigNum1 + bigNum2;
    EXPECT_EQ(result, BigInt("11111111101111111110111111111011111111100"));
}

TEST_F(BigIntTest, VeryLargeSubtractionWorks) {
    BigInt result = bigNum2 - bigNum1;
    EXPECT_EQ(result, BigInt("8641975320864197532086419753208641975320"));
}


TEST_F(BigIntTest, VeryLargeKaratsubaMultiplication) {
    BigInt result = bigNum1.karatsuba_multiply(bigNum2);
    EXPECT_EQ(result, BigInt("12193263113702179522618503273386678859448712086533622923332237463801111263526900"));
    
}

TEST_F(BigIntTest, VeryLargeKaratsubaMultiplication1) {
  
    BigInt result = bigNum5.karatsuba_multiply(bigNum6);
    EXPECT_EQ(result, BigInt("121932631137021795226185032733866788594511507391563633592367367779295611949397448712086533622923332237463801111263526900"));
}

TEST_F(BigIntTest, VeryLargeFFTMultiplication) {
    BigInt result = bigNum1.fft_multiply(bigNum2);
    EXPECT_EQ(result, BigInt("12193263113702179522618503273386678859448712086533622923332237463801111263526900"));
}

TEST_F(BigIntTest, VeryLargeFFTMultiplication1) {
    BigInt result = bigNum5.fft_multiply(bigNum6);
    EXPECT_EQ(result, BigInt("121932631137021795226185032733866788594511507391563633592367367779295611949397448712086533622923332237463801111263526900"));
}

TEST_F(BigIntTest, VeryLargeFFTMultiplication2) {
    BigInt a("100000000000000000000000000000000000");
    BigInt b("10000000000000000000");

    BigInt result = a.fft_multiply(b);


    EXPECT_EQ(result, BigInt("1000000000000000000000000000000000000000000000000000000"));
}

TEST_F(BigIntTest, VeryLargeFFTMultiplication3) {
    BigInt a("0");
    BigInt b("0");

    BigInt result = a.fft_multiply(b);


    EXPECT_EQ(result, BigInt("0"));
}

TEST_F(BigIntTest, VeryLargeMultiplicationWorks) {
    BigInt result = bigNum1 * bigNum2;
    EXPECT_EQ(result, BigInt("12193263113702179522618503273386678859448712086533622923332237463801111263526900"));
}

TEST_F(BigIntTest, VeryLargeDivisionWorks) {
    BigInt result = bigNum4 / bigNum1;
    EXPECT_EQ(result, BigInt("8"));
}

TEST_F(BigIntTest, VeryLargeModulusWorks) {
    BigInt result = bigNum4 % bigNum1;
    EXPECT_EQ(result, BigInt("123456879012345687901234568790123456880"));
}

TEST_F(BigIntTest, LeftShiftWorks) {
    EXPECT_EQ(pos123 << BigInt(2), BigInt(492));
}

TEST_F(BigIntTest, RightShiftWorks) {
    EXPECT_EQ(BigInt(492) >> BigInt(2), pos123);
}

TEST_F(BigIntTest, CompoundAdditionWorks) {
    BigInt num = pos123;
    num += BigInt(377);
    EXPECT_EQ(num, BigInt(500));
}

TEST_F(BigIntTest, CompoundSubtractionWorks) {
    BigInt num = pos123;
    num -= BigInt(23);
    EXPECT_EQ(num, BigInt(100));
}

TEST_F(BigIntTest, CompoundMultiplicationWorks) {
    BigInt num = pos123;
    num *= BigInt(2);
    EXPECT_EQ(num, BigInt(246));
}

TEST_F(BigIntTest, CompoundDivisionWorks) {
    BigInt num = largePos;
    num /= BigInt(10);
    EXPECT_EQ(num, BigInt("1234567890123456789"));
}

TEST_F(BigIntTest, CompoundModulusWorks) {
    BigInt num = pos123;
    num %= BigInt(100);
    EXPECT_EQ(num, BigInt(23));
}

TEST_F(BigIntTest, CompoundLeftShiftWorks) {
    BigInt num = pos123;
    num <<= BigInt(2);
    EXPECT_EQ(num, BigInt(492));
}

TEST_F(BigIntTest, CompoundRightShiftWorks) {
    BigInt num = BigInt(492);
    num >>= BigInt(2);
    EXPECT_EQ(num, pos123);
}

TEST_F(BigIntTest, PrefixIncrementWorks) {
    BigInt num = pos123;
    EXPECT_EQ(++num, BigInt(124));
}

TEST_F(BigIntTest, PostfixIncrementWorks) {
    BigInt num = pos123;
    EXPECT_EQ(num++, pos123);
    EXPECT_EQ(num, BigInt(124));
}

TEST_F(BigIntTest, PrefixDecrementWorks) {
    BigInt num = pos123;
    EXPECT_EQ(--num, BigInt(122));
}

TEST_F(BigIntTest, PostfixDecrementWorks) {
    BigInt num = pos123;
    EXPECT_EQ(num--, pos123);
    EXPECT_EQ(num, BigInt(122));
}

TEST_F(BigIntTest, EqualityOperatorWorks) {
    EXPECT_TRUE(pos123 == BigInt(123));
}

TEST_F(BigIntTest, InequalityOperatorWorks) {
    EXPECT_TRUE(pos123 != neg456);
}

TEST_F(BigIntTest, LessThanOperatorWorks) {
    EXPECT_TRUE(neg456 < pos123);
}

TEST_F(BigIntTest, GreaterThanOperatorWorks) {
    EXPECT_TRUE(pos123 > neg456);
}

TEST_F(BigIntTest, LessOrEqualOperatorWorks) {
    EXPECT_TRUE(neg456 <= pos123);
}

TEST_F(BigIntTest, GreaterOrEqualOperatorWorks) {
    EXPECT_TRUE(pos123 >= neg456);
}

TEST_F(BigIntTest, SwapFunctionWorks) {
    BigInt a = pos123;
    BigInt b = neg456;
    BigInt::swap(a, b);
    EXPECT_EQ(a, neg456);
    EXPECT_EQ(b, pos123);
}

TEST_F(BigIntTest, OutputOperatorWorks) {
    std::ostringstream oss;
    oss << pos123;
    EXPECT_EQ(oss.str(), "123");
}

TEST_F(BigIntTest, InputOperatorWorks) {
    std::istringstream iss("123");
    BigInt num;
    iss >> num;
    EXPECT_EQ(num, pos123);
}

TEST_F(BigIntTest, ModularExponentiationWithoutModWorks) {
    EXPECT_EQ(BigInt::BigInt_mod_exp(BigInt(2), BigInt(10)), BigInt(1024));
}

TEST_F(BigIntTest, ModularExponentiationWithModWorks) {
    EXPECT_EQ(BigInt::BigInt_mod_exp(BigInt(2), BigInt(10), BigInt(1000)), BigInt(24));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}