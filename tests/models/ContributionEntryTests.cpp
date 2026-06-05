#include <gtest/gtest.h>
#include "../../src/models/ContributionEntry.hpp"

namespace {
    ContributionEntry makeEntry() {
        return ContributionEntry(
            1, 1, 1,
            ContributionType::Pension,
            488.0,   // 9.76% of 5000
            488.0    // 9.76% of 5000
        );
    }
}


TEST(ContributionEntryTest, ValidEntry) {
    EXPECT_NO_THROW(makeEntry());
}

TEST(ContributionEntryTest, NegativeAmountInsuredThrows) {
    EXPECT_THROW(
        ContributionEntry(1, 1, 1, ContributionType::Pension, -1.0, 488.0),
        std::invalid_argument
    );
}

TEST(ContributionEntryTest, NegativeAmountPayerThrows) {
    EXPECT_THROW(
        ContributionEntry(1, 1, 1, ContributionType::Pension, 488.0, -1.0),
        std::invalid_argument
    );
}

TEST(ContributionEntryTest, ZeroAmountsAllowed) {
    EXPECT_NO_THROW(
        ContributionEntry(1, 1, 1, ContributionType::HealthInsurance, 0.0, 0.0)
    );
}


TEST(ContributionEntryTest, GetType) {
    EXPECT_EQ(makeEntry().getType(), ContributionType::Pension);
}

TEST(ContributionEntryTest, GetAmountInsured) {
    EXPECT_DOUBLE_EQ(makeEntry().getAmountInsured(), 488.0);
}

TEST(ContributionEntryTest, GetAmountPayer) {
    EXPECT_DOUBLE_EQ(makeEntry().getAmountPayer(), 488.0);
}


TEST(ContributionEntryTest, TotalAmount) {
    EXPECT_DOUBLE_EQ(makeEntry().totalAmount(), 976.0);
}

TEST(ContributionEntryTest, TotalAmountAsymmetric) {
    ContributionEntry e(1, 1, 1, ContributionType::Accident, 0.0, 83.5);
    EXPECT_DOUBLE_EQ(e.totalAmount(), 83.5);
}