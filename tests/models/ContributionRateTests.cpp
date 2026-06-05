#include <gtest/gtest.h>
#include "../../src/models/ContributionRate.hpp"

namespace {
    ContributionRate makeRate() {
        return ContributionRate(
            1,
            ContributionType::Pension,
            9.76,   // employee
            9.76,   // employer
            Date(1, 1, 2024),
            Date(31, 12, 2024)
        );
    }
}

TEST(ContributionRateTest, ValidRate) {
    EXPECT_NO_THROW(makeRate());
}

TEST(ContributionRateTest, NegativeRateInsuredThrows) {
    EXPECT_THROW(
        ContributionRate(1, ContributionType::Pension, -1.0, 9.76,
                         Date(1, 1, 2024), Date(31, 12, 2024)),
        std::invalid_argument
    );
}

TEST(ContributionRateTest, RateInsuredOver100Throws) {
    EXPECT_THROW(
        ContributionRate(1, ContributionType::Pension, 101.0, 9.76,
                         Date(1, 1, 2024), Date(31, 12, 2024)),
        std::invalid_argument
    );
}

TEST(ContributionRateTest, ValidFromAfterValidToThrows) {
    EXPECT_THROW(
        ContributionRate(1, ContributionType::Pension, 9.76, 9.76,
                         Date(31, 12, 2024), Date(1, 1, 2024)),
        std::invalid_argument
    );
}

TEST(ContributionRateTest, ActiveForDateInRange) {
    EXPECT_TRUE(makeRate().isActiveFor(Date(15, 6, 2024)));
}

TEST(ContributionRateTest, ActiveForStartDate) {
    EXPECT_TRUE(makeRate().isActiveFor(Date(1, 1, 2024)));
}

TEST(ContributionRateTest, ActiveForEndDate) {
    EXPECT_TRUE(makeRate().isActiveFor(Date(31, 12, 2024)));
}

TEST(ContributionRateTest, NotActiveBeforeRange) {
    EXPECT_FALSE(makeRate().isActiveFor(Date(31, 12, 2023)));
}

TEST(ContributionRateTest, NotActiveAfterRange) {
    EXPECT_FALSE(makeRate().isActiveFor(Date(1, 1, 2025)));
}


TEST(ContributionRateTest, TotalRate) {
    EXPECT_DOUBLE_EQ(makeRate().totalRate(), 19.52);
}