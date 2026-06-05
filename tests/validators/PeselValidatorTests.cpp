#include <gtest/gtest.h>

#include "../src/validators/PeselValidator.hpp"

TEST(PeselValidatorTest, PeselToShort) {
    EXPECT_FALSE(PeselValidator::isValid("123123123"));
}

TEST(PeselValidatorTest, PeselEmpty) {
    EXPECT_FALSE(PeselValidator::isValid(""));
}

TEST(PeselValidatorTest, PeselContainsLetter) {
    EXPECT_FALSE(PeselValidator::isValid("9704134750L"));
}

TEST(PeselValidatorTest, PeselToLong) {
    EXPECT_FALSE(PeselValidator::isValid("123123123123"));
}

TEST(PeselValidatorTest, PeselNotCorrect) {
    EXPECT_FALSE(PeselValidator::isValid("12312312312"));
}

TEST(PeselValidatorTest, PeselCorrect) {
    EXPECT_TRUE(PeselValidator::isValid("97041347508")); // random CORRECT PESEL
}

TEST(PeselValidatorTest, PeselCorrespondsToDateOfBirth) {
    EXPECT_TRUE(PeselValidator::isPESELCorrespondsToDateOfBirth("66020200804", {2, 2, 1966}));
}

TEST(PeselValidatorTest, IncorrectPeselCorrespondsToDateOfBirth) {
    EXPECT_FALSE(PeselValidator::isPESELCorrespondsToDateOfBirth("66020200000", {2, 2, 1966}));
}

TEST(PeselValidatorTest, PeselNotCorrespondsToDateOfBirth) {
    EXPECT_FALSE(PeselValidator::isPESELCorrespondsToDateOfBirth("97041347508", {9, 6, 2102}));
}