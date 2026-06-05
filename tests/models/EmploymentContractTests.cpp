#include <gtest/gtest.h>
#include "../../src/models/EmploymentContract.hpp"

namespace {
    EmploymentContract makeContract() {
        return EmploymentContract(
            1, 1, 1,
            ContractType::UoP,
            5000.0,
            Date(1, 1, 2024),
            Date(31, 12, 2024)
        );
    }
}


TEST(EmploymentContractTest, ValidContract) {
    EXPECT_NO_THROW(makeContract());
}

TEST(EmploymentContractTest, NegativeSalaryThrows) {
    EXPECT_THROW(
        EmploymentContract(1, 1, 1, ContractType::UoP, -100.0,
                           Date(1, 1, 2024), Date(31, 12, 2024)),
        std::invalid_argument
    );
}

TEST(EmploymentContractTest, ZeroSalaryThrows) {
    EXPECT_THROW(
        EmploymentContract(1, 1, 1, ContractType::UoP, 0.0,
                           Date(1, 1, 2024), Date(31, 12, 2024)),
        std::invalid_argument
    );
}

TEST(EmploymentContractTest, EndBeforeStartThrows) {
    EXPECT_THROW(
        EmploymentContract(1, 1, 1, ContractType::UoP, 5000.0,
                           Date(1, 6, 2024), Date(1, 1, 2024)),
        std::invalid_argument
    );
}

TEST(EmploymentContractTest, SameDateThrows) {
    EXPECT_THROW(
        EmploymentContract(1, 1, 1, ContractType::UoP, 5000.0,
                           Date(1, 1, 2024), Date(1, 1, 2024)),
        std::invalid_argument
    );
}


TEST(EmploymentContractTest, GetType) {
    EXPECT_EQ(makeContract().getType(), ContractType::UoP);
}

TEST(EmploymentContractTest, GetGrossSalary) {
    EXPECT_DOUBLE_EQ(makeContract().getGrossSalary(), 5000.0);
}

TEST(EmploymentContractTest, InitialStatusIsActive) {
    EXPECT_TRUE(makeContract().isActive());
}

TEST(EmploymentContractTest, TerminateContract) {
    auto c = makeContract();
    c.terminate(Date(30, 6, 2024));
    EXPECT_EQ(c.getStatus(), ContractStatus::Terminated);
    EXPECT_FALSE(c.isActive());
}

TEST(EmploymentContractTest, TerminateSetsEndDate) {
    auto c = makeContract();
    c.terminate(Date(30, 6, 2024));
    EXPECT_EQ(c.getEndDate(), Date(30, 6, 2024));
}

TEST(EmploymentContractTest, TerminateAlreadyTerminatedThrows) {
    auto c = makeContract();
    c.terminate(Date(30, 6, 2024));
    EXPECT_THROW(c.terminate(Date(30, 9, 2024)), std::logic_error);
}

TEST(EmploymentContractTest, TerminateBeforeStartThrows) {
    auto c = makeContract();
    EXPECT_THROW(c.terminate(Date(1, 12, 2023)), std::invalid_argument);
}


TEST(EmploymentContractTest, ExpireContract) {
    auto c = makeContract();
    c.expire();
    EXPECT_EQ(c.getStatus(), ContractStatus::Expired);
}

TEST(EmploymentContractTest, ExpireAlreadyExpiredThrows) {
    auto c = makeContract();
    c.expire();
    EXPECT_THROW(c.expire(), std::logic_error);
}