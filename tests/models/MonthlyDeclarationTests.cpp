#include <gtest/gtest.h>
#include "../../src/models/MonthlyDeclaration.hpp"
#include "../../src/services/ContributionCalculator.hpp"
#include "../../src/services/UoPStrategy.hpp"

namespace {
    std::vector<ContributionRate> makeRates() {
        return {
            ContributionRate(1, ContributionType::Pension,         9.76, 9.76, Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(2, ContributionType::Disability,      1.5,  6.5,  Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(3, ContributionType::Sickness,        2.45, 0.0,  Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(4, ContributionType::Accident,        0.0,  1.67, Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(5, ContributionType::HealthInsurance, 9.0,  0.0,  Date(1,1,2024), Date(31,12,2024)),
            ContributionRate(6, ContributionType::LaborFund,       0.0,  2.45, Date(1,1,2024), Date(31,12,2024)),
        };
    }

    MonthlyDeclaration makeDeclaration() {
        return MonthlyDeclaration(1, 1, Date(1, 1, 2024), 5000.0);
    }

    MonthlyDeclaration makeDeclarationWithEntries() {
        auto decl = makeDeclaration();
        ContributionCalculator calc(std::make_shared<UoPStrategy>());
        decl.setEntries(calc.calculate(1, 5000.0, makeRates()));
        return decl;
    }
}


TEST(MonthlyDeclarationTest, ValidDeclaration) {
    EXPECT_NO_THROW(makeDeclaration());
}

TEST(MonthlyDeclarationTest, ZeroGrossBasisThrows) {
    EXPECT_THROW(
        MonthlyDeclaration(1, 1, Date(1, 1, 2024), 0.0),
        std::invalid_argument
    );
}

TEST(MonthlyDeclarationTest, InitialStatusIsDraft) {
    EXPECT_EQ(makeDeclaration().getStatus(), DeclarationStatus::Draft);
}


TEST(MonthlyDeclarationTest, SetEntries) {
    auto decl = makeDeclaration();
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    EXPECT_NO_THROW(decl.setEntries(calc.calculate(1, 5000.0, makeRates())));
}

TEST(MonthlyDeclarationTest, SetEmptyEntriesThrows) {
    auto decl = makeDeclaration();
    EXPECT_THROW(decl.setEntries({}), std::invalid_argument);
}

TEST(MonthlyDeclarationTest, SetEntriesOnSubmittedThrows) {
    auto decl = makeDeclarationWithEntries();
    decl.submit();
    ContributionCalculator calc(std::make_shared<UoPStrategy>());
    EXPECT_THROW(
        decl.setEntries(calc.calculate(1, 5000.0, makeRates())),
        std::logic_error
    );
}


TEST(MonthlyDeclarationTest, SubmitDraft) {
    auto decl = makeDeclarationWithEntries();
    decl.submit();
    EXPECT_EQ(decl.getStatus(), DeclarationStatus::Submitted);
}

TEST(MonthlyDeclarationTest, SubmitWithoutEntriesThrows) {
    auto decl = makeDeclaration();
    EXPECT_THROW(decl.submit(), std::logic_error);
}

TEST(MonthlyDeclarationTest, SubmitAlreadySubmittedThrows) {
    auto decl = makeDeclarationWithEntries();
    decl.submit();
    EXPECT_THROW(decl.submit(), std::logic_error);
}

TEST(MonthlyDeclarationTest, AcceptSubmitted) {
    auto decl = makeDeclarationWithEntries();
    decl.submit();
    decl.accept();
    EXPECT_EQ(decl.getStatus(), DeclarationStatus::Accepted);
}

TEST(MonthlyDeclarationTest, AcceptDraftThrows) {
    auto decl = makeDeclarationWithEntries();
    EXPECT_THROW(decl.accept(), std::logic_error);
}

TEST(MonthlyDeclarationTest, CorrectAccepted) {
    auto decl = makeDeclarationWithEntries();
    decl.submit();
    decl.accept();
    decl.correct();
    EXPECT_EQ(decl.getStatus(), DeclarationStatus::Corrected);
}

TEST(MonthlyDeclarationTest, CorrectSubmittedThrows) {
    auto decl = makeDeclarationWithEntries();
    decl.submit();
    EXPECT_THROW(decl.correct(), std::logic_error);
}


TEST(MonthlyDeclarationTest, TotalInsured) {
    auto decl = makeDeclarationWithEntries();
    // Pension: 488.0 + Disability: 75.0 + Sickness: 122.5 + Health: 450.0 = 1135.5
    EXPECT_NEAR(decl.totalInsured(), 1135.5, 0.01);
}

TEST(MonthlyDeclarationTest, TotalPayer) {
    auto decl = makeDeclarationWithEntries();
    // Pension: 488.0 + Disability: 325.0 + Accident: 83.5 + LaborFund: 122.5 = 1019.0
    EXPECT_NEAR(decl.totalPayer(), 1019.0, 0.01);
}

TEST(MonthlyDeclarationTest, TotalAmount) {
    auto decl = makeDeclarationWithEntries();
    EXPECT_NEAR(decl.totalAmount(), decl.totalInsured() + decl.totalPayer(), 0.01);
}