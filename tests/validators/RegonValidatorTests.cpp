#include <gtest/gtest.h>
#include "../../src/validators/RegonValidator.hpp"


TEST(RegonValidatorTest, TooShort) {
    EXPECT_FALSE(RegonValidator::isValid("1234567"));
}

TEST(RegonValidatorTest, TooLong) {
    EXPECT_FALSE(RegonValidator::isValid("123456789012345"));
}

TEST(RegonValidatorTest, WrongLength10) {
    EXPECT_FALSE(RegonValidator::isValid("1234567890"));
}

TEST(RegonValidatorTest, Empty) {
    EXPECT_FALSE(RegonValidator::isValid(""));
}

TEST(RegonValidatorTest, ContainsLetters) {
    EXPECT_FALSE(RegonValidator::isValid("12345678X"));
}


TEST(RegonValidatorTest, Valid9Digit) {
    EXPECT_TRUE(RegonValidator::isValid("123456785"));
}

TEST(RegonValidatorTest, Invalid9Digit) {
    EXPECT_FALSE(RegonValidator::isValid("123456789"));
}


TEST(RegonValidatorTest, Valid14Digit) {
    EXPECT_TRUE(RegonValidator::isValid("12345678512347"));
}

TEST(RegonValidatorTest, Invalid14Digit) {
    EXPECT_FALSE(RegonValidator::isValid("12345678512340"));
}