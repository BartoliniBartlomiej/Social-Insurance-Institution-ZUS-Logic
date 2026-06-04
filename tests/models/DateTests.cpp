#include <gtest/gtest.h>
#include "../../src/models/Date.hpp"

TEST(DateTest, ValidDate) {
    EXPECT_NO_THROW(Date(15, 6, 1990));
}

TEST(DateTest, InvalidMonth) {
    EXPECT_THROW(Date(1, 13, 1990), std::invalid_argument);
}

TEST(DateTest, InvalidDay) {
    EXPECT_THROW(Date(32, 1, 1990), std::invalid_argument);
}

TEST(DateTest, InvalidDayForMonth) {
    EXPECT_THROW(Date(31, 4, 1990), std::invalid_argument);
}

TEST(DateTest, LeapYearFebruary) {
    EXPECT_NO_THROW(Date(29, 2, 2000));
}

TEST(DateTest, NonLeapYearFebruary) {
    EXPECT_THROW(Date(29, 2, 1900), std::invalid_argument);
}

TEST(DateTest, ToString) {
    Date d(5, 3, 1990);
    EXPECT_EQ(d.toString(), "05.03.1990");
}

TEST(DateTest, EqualityOperator) {
    EXPECT_EQ(Date(1, 1, 2000), Date(1, 1, 2000));
}

TEST(DateTest, LessThanOperator) {
    EXPECT_LT(Date(1, 1, 2000), Date(2, 1, 2000));
}