#include <gtest/gtest.h>
#include "../../src/models/Citizen.hpp"

namespace {
    Citizen makeCitizen() {
        return Citizen(
            1,
            "Jan", "Marek", "Kowalski",
            "90061512345",
            Date(15, 6, 1990),
            Address("ul. Kowalska 1", "Warszawa", "00-001")
        );
    }
}

TEST(CitizenTest, FullNameWithSecondName) {
    EXPECT_EQ(makeCitizen().getFullName(), "Jan Marek Kowalski");
}

TEST(CitizenTest, FullNameWithoutSecondName) {
    Citizen c(1, "Jan", "", "Kowalski", "90061512345",
              Date(15, 6, 1990),
              Address("ul. Kowalska 1", "Warszawa", "00-001"));
    EXPECT_EQ(c.getFullName(), "Jan Kowalski");
}

TEST(CitizenTest, GetPesel) {
    EXPECT_EQ(makeCitizen().getPesel(), "90061512345");
}

TEST(CitizenTest, GetDateBirth) {
    EXPECT_EQ(makeCitizen().getDateBirth(), Date(15, 6, 1990));
}

TEST(CitizenTest, EmptyPeselThrows) {
    EXPECT_THROW(
        Citizen(1, "Jan", "", "Kowalski", "",
                Date(15, 6, 1990),
                Address("ul. Kowalska 1", "Warszawa", "00-001")),
        std::invalid_argument
    );
}

TEST(CitizenTest, UpdateAddress) {
    Citizen c = makeCitizen();
    Address newAddr("ul. Nowa 5", "Kraków", "30-001");
    c.updateAddress(newAddr);
    EXPECT_EQ(c.getAddress().getCity(), "Kraków");
}