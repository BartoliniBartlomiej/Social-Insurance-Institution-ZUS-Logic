#include <gtest/gtest.h>
#include <memory>
#include "../../src/services/ContributionCalculator.hpp"
#include "../../src/services/UoPStrategy.hpp"
#include "../../src/services/B2BStrategy.hpp"
#include "../../src/services/UoZStrategy.hpp"

namespace {
    std::vector<ContributionRate> makeRates() {
        return {
            ContributionRate(1, ContributionType::Pension,         9.76,  9.76,  Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(2, ContributionType::Disability,      1.5,   6.5,   Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(3, ContributionType::Sickness,        2.45,  0.0,   Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(4, ContributionType::Accident,        0.0,   1.67,  Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(5, ContributionType::HealthInsurance, 9.0,   0.0,   Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(6, ContributionType::LaborFund,       0.0,   2.45,  Date(1,1,2024), Date(31,12,2024)),
        };
    }
}


TEST(ContributionCalculatorTest, NullStrategyThrows) {
    EXPECT_THROW(
        ContributionCalculator(nullptr),
        std::invalid_argument
    );
}


TEST(ContributionCalculatorTest, UoPReturnsAllEntries) {
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    EXPECT_EQ(entries.size(), 6);
}

TEST(ContributionCalculatorTest, UoPPensionInsured) {
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    EXPECT_NEAR(entries[0].getAmountInsured(), 488.0, 0.01);
}

TEST(ContributionCalculatorTest, UoPAccidentInsuredIsZero) {
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    auto accident = *std::find_if(entries.begin(), entries.end(),
        [](const ContributionEntry& e) {
            return e.getType() == ContributionType::Accident;
        });
    EXPECT_DOUBLE_EQ(accident.getAmountInsured(), 0.0);
    EXPECT_NEAR(accident.getAmountPayer(), 83.5, 0.01);
}


TEST(ContributionCalculatorTest, B2BPensionPaidBySelf) {
    ContributionCalculator calc(std::make_shared<B2BStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    EXPECT_NEAR(entries[0].getAmountInsured(), 976.0, 0.01);
    EXPECT_DOUBLE_EQ(entries[0].getAmountPayer(), 0.0);
}

TEST(ContributionCalculatorTest, B2BPayerAlwaysZero) {
    ContributionCalculator calc(std::make_shared<B2BStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    for (const auto& entry : entries) {
        EXPECT_DOUBLE_EQ(entry.getAmountPayer(), 0.0);
    }
}


TEST(ContributionCalculatorTest, StrategySwap) {
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    auto uopEntries = calc.calculate(1, 5000.0, makeRates());

    calc.setStrategy(std::make_shared<B2BStrategy>());
    auto b2bEntries = calc.calculate(1, 5000.0, makeRates());

    EXPECT_NE(uopEntries[0].getAmountInsured(), b2bEntries[0].getAmountInsured());
}


TEST(ContributionCalculatorTest, ZeroGrossBasisThrows) {
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    EXPECT_THROW(calc.calculate(1, 0.0, makeRates()), std::invalid_argument);
}

TEST(ContributionCalculatorTest, EmptyRatesThrows) {
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    EXPECT_THROW(calc.calculate(1, 5000.0, {}), std::invalid_argument);
}



TEST(ContributionCalculatorTest, UoZLaborFundIsZero) {
    ContributionCalculator calc(std::make_shared<UoZStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    auto laborFund = *std::find_if(entries.begin(), entries.end(),
        [](const ContributionEntry& e) {
            return e.getType() == ContributionType::LaborFund;
        });
    EXPECT_DOUBLE_EQ(laborFund.getAmountInsured(), 0.0);
    EXPECT_DOUBLE_EQ(laborFund.getAmountPayer(), 0.0);
}

TEST(ContributionCalculatorTest, UoZPensionObligatory) {
    ContributionCalculator calc(std::make_shared<UoZStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    auto pension = *std::find_if(entries.begin(), entries.end(),
        [](const ContributionEntry& e) {
            return e.getType() == ContributionType::Pension;
        });
    EXPECT_NEAR(pension.getAmountInsured(), 488.0, 0.01);
    EXPECT_NEAR(pension.getAmountPayer(), 488.0, 0.01);
}

TEST(ContributionCalculatorTest, UoZSicknessOnlyInsured) {
    ContributionCalculator calc(std::make_shared<UoZStrategy>());
    auto entries = calc.calculate(1, 5000.0, makeRates());
    auto sickness = *std::find_if(entries.begin(), entries.end(),
        [](const ContributionEntry& e) {
            return e.getType() == ContributionType::Sickness;
        });
    EXPECT_NEAR(sickness.getAmountInsured(), 122.5, 0.01);
    EXPECT_DOUBLE_EQ(sickness.getAmountPayer(), 0.0);
}