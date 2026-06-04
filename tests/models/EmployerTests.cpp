#include <gtest/gtest.h>
#include "../../src/models/Employer.hpp"

namespace {
    Employer makeEmployer() {
        return Employer(
            1,
            "Acme Sp. z o.o.",
            "1234563218",
            "123456785",
            AccidentRiskCategory::Medium,
            Address("ul. Biznesowa 10", "Kraków", "30-001")
        );
    }
}

TEST(EmployerTest, GetFullName) {
    EXPECT_EQ(makeEmployer().getFullName(), "Acme Sp. z o.o.");
}

TEST(EmployerTest, GetNip) {
    EXPECT_EQ(makeEmployer().getNip(), "1234563218");
}

TEST(EmployerTest, GetRiskCategory) {
    EXPECT_EQ(makeEmployer().getRiskCategory(), AccidentRiskCategory::Medium);
}

TEST(EmployerTest, EmptyNipThrows) {
    EXPECT_THROW(
        Employer(1, "Acme", "", "123456785",
                 AccidentRiskCategory::Low,
                 Address("ul. Biznesowa 10", "Kraków", "30-001")),
        std::invalid_argument
    );
}

TEST(EmployerTest, EmptyRegonThrows) {
    EXPECT_THROW(
        Employer(1, "Acme", "1234563218", "",
                 AccidentRiskCategory::Low,
                 Address("ul. Biznesowa 10", "Kraków", "30-001")),
        std::invalid_argument
    );
}

TEST(EmployerTest, UpdateAddress) {
    Employer e = makeEmployer();
    e.updateAddress(Address("ul. Nowa 1", "Warszawa", "00-001"));
    EXPECT_EQ(e.getAddress().getCity(), "Warszawa");
}