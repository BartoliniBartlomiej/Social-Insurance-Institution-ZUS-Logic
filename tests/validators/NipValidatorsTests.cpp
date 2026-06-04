#include <gtest/gtest.h>
#include "../../src/validators/NipValidator.hpp"

TEST(NipValidatorTest, TooShort) {
    EXPECT_FALSE(NipValidator::isValid("123456321"));
}

TEST(NipValidatorTest, TooLong) {
    EXPECT_FALSE(NipValidator::isValid("12345632189"));
}

TEST(NipValidatorTest, ContainsLetters) {
    EXPECT_FALSE(NipValidator::isValid("123456321X"));
}

TEST(NipValidatorTest, Empty) {
    EXPECT_FALSE(NipValidator::isValid(""));
}

TEST(NipValidatorTest, WrongChecksum) {
    EXPECT_FALSE(NipValidator::isValid("1234563219"));
}

TEST(NipValidatorTest, Valid) {
    EXPECT_TRUE(NipValidator::isValid("1234563218"));
}