#include <gtest/gtest.h>
#include "../../src/services/CitizenService.hpp"
#include "../fakes/InMemoryCitizenRepository.hpp"

namespace {
    Citizen makeCitizen(int id = 1, const std::string& pesel = "97041347508") {
        return Citizen(
            id, "Jan", "Marek", "Kowalski",
            pesel,
            Date(13, 4, 1997),
            Address("ul. Kowalska 1", "Warszawa", "00-001")
        );
    }
}

// ── registerCitizen ───────────────────────────────────────────────────────────

TEST(CitizenServiceTest, RegisterValidCitizen) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    EXPECT_NO_THROW(service.registerCitizen(makeCitizen()));
    EXPECT_EQ(repo.size(), 1);
}

TEST(CitizenServiceTest, RegisterInvalidPeselThrows) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    Citizen invalid(1, "Jan", "", "Kowalski", "12345678901",
                    Date(1, 1, 1990),
                    Address("ul. Kowalska 1", "Warszawa", "00-001"));

    EXPECT_THROW(service.registerCitizen(invalid), std::invalid_argument);
    EXPECT_EQ(repo.size(), 0);
}

TEST(CitizenServiceTest, RegisterDuplicatePeselThrows) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    service.registerCitizen(makeCitizen(1));
    EXPECT_THROW(service.registerCitizen(makeCitizen(2)), std::logic_error);
    EXPECT_EQ(repo.size(), 1);
}

// ── getCitizen ────────────────────────────────────────────────────────────────

TEST(CitizenServiceTest, GetExistingCitizen) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    service.registerCitizen(makeCitizen());
    auto c = service.getCitizen(1);
    EXPECT_EQ(c.getFullName(), "Jan Marek Kowalski");
}

TEST(CitizenServiceTest, GetNonExistingCitizenThrows) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    EXPECT_THROW(service.getCitizen(99), std::logic_error);
}

// ── getCitizenByPesel ─────────────────────────────────────────────────────────

TEST(CitizenServiceTest, GetByValidPesel) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    service.registerCitizen(makeCitizen());
    auto c = service.getCitizenByPesel("97041347508");
    EXPECT_EQ(c.getId(), 1);
}

TEST(CitizenServiceTest, GetByInvalidPeselThrows) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    EXPECT_THROW(service.getCitizenByPesel("12345678901"), std::invalid_argument);
}

// ── updateAddress ─────────────────────────────────────────────────────────────

TEST(CitizenServiceTest, UpdateAddress) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    service.registerCitizen(makeCitizen());
    service.updateAddress(1, Address("ul. Nowa 5", "Kraków", "30-001"));

    auto c = service.getCitizen(1);
    EXPECT_EQ(c.getAddress().getCity(), "Kraków");
}

TEST(CitizenServiceTest, UpdateAddressNonExistingThrows) {
    InMemoryCitizenRepository repo;
    CitizenService service(repo);

    EXPECT_THROW(
        service.updateAddress(99, Address("ul. Nowa 5", "Kraków", "30-001")),
        std::logic_error
    );
}