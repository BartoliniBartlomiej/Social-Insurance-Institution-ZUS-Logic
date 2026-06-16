#include <gtest/gtest.h>
#include "../../src/services/DeclarationService.hpp"
#include "../fakes/InMemoryContractRepository.hpp"
#include "../fakes/InMemoryDeclarationRepository.hpp"

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

    EmploymentContract makeContract(ContractType type = ContractType::UoP) {
        return EmploymentContract(
            1, 1, 1, type, 5000.0,
            Date(1, 1, 2024), Date(31, 12, 2024)
        );
    }
}

// ── createDeclaration ─────────────────────────────────────────────────────────

TEST(DeclarationServiceTest, CreateDeclarationUoP) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    contractRepo.save(makeContract());
    auto decl = service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates());

    EXPECT_EQ(decl.getContractId(), 1);
    EXPECT_EQ(decl.getStatus(), DeclarationStatus::Draft);
    EXPECT_EQ(declarationRepo.size(), 1);
}

TEST(DeclarationServiceTest, CreateDeclarationB2B) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    contractRepo.save(makeContract(ContractType::B2B));
    auto decl = service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates());

    // B2B — payer zawsze 0
    for (const auto& entry : decl.getEntries())
        EXPECT_DOUBLE_EQ(entry.getAmountPayer(), 0.0);
}

TEST(DeclarationServiceTest, CreateDeclarationContractNotFoundThrows) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    EXPECT_THROW(
        service.createDeclaration(1, 99, Date(1, 1, 2024), makeRates()),
        std::logic_error
    );
}

TEST(DeclarationServiceTest, CreateDuplicateDeclarationThrows) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    contractRepo.save(makeContract());
    service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates());

    EXPECT_THROW(
        service.createDeclaration(2, 1, Date(1, 1, 2024), makeRates()),
        std::logic_error
    );
}

TEST(DeclarationServiceTest, CreateDeclarationInactiveContractThrows) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    auto contract = makeContract();
    contract.terminate(Date(30, 6, 2024));
    contractRepo.save(contract);

    EXPECT_THROW(
        service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates()),
        std::logic_error
    );
}

// ── submitDeclaration ─────────────────────────────────────────────────────────

TEST(DeclarationServiceTest, SubmitDeclaration) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    contractRepo.save(makeContract());
    service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates());
    service.submitDeclaration(1);

    EXPECT_EQ(service.getDeclaration(1).getStatus(), DeclarationStatus::Submitted);
}

TEST(DeclarationServiceTest, SubmitNonExistingThrows) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    EXPECT_THROW(service.submitDeclaration(99), std::logic_error);
}

// ── acceptDeclaration ─────────────────────────────────────────────────────────

TEST(DeclarationServiceTest, AcceptDeclaration) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    contractRepo.save(makeContract());
    service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates());
    service.submitDeclaration(1);
    service.acceptDeclaration(1);

    EXPECT_EQ(service.getDeclaration(1).getStatus(), DeclarationStatus::Accepted);
}

TEST(DeclarationServiceTest, AcceptWithoutSubmitThrows) {
    InMemoryContractRepository contractRepo;
    InMemoryDeclarationRepository declarationRepo;
    DeclarationService service(declarationRepo, contractRepo);

    contractRepo.save(makeContract());
    service.createDeclaration(1, 1, Date(1, 1, 2024), makeRates());

    EXPECT_THROW(service.acceptDeclaration(1), std::logic_error);
}