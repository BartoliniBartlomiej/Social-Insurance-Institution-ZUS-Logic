#pragma once

#include <memory>
#include <stdexcept>
#include "../models/MonthlyDeclaration.hpp"
#include "../models/EmploymentContract.hpp"
#include "../repositories/IDeclarationRepository.hpp"
#include "../repositories/IContractRepository.hpp"
#include "ContributionCalculator.hpp"
#include "UoPStrategy.hpp"
#include "UoZStrategy.hpp"
#include "B2BStrategy.hpp"

class DeclarationService {
private:
    IDeclarationRepository& declarationRepo;
    IContractRepository&    contractRepo;

    std::shared_ptr<ICalculationStrategy> strategyFor(ContractType type) const {
        switch (type) {
            case ContractType::UoP: return std::make_shared<UoPStrategy>();
            case ContractType::UoZ: return std::make_shared<UoZStrategy>();
            case ContractType::B2B: return std::make_shared<B2BStrategy>();
        }
        throw std::logic_error("Unknown contract type");
    }

public:
    DeclarationService(
        IDeclarationRepository& _declarationRepo,
        IContractRepository&    _contractRepo
    )
        : declarationRepo(_declarationRepo)
        , contractRepo(_contractRepo)
    {}

    MonthlyDeclaration createDeclaration(
        int                                  declarationId,
        int                                  contractId,
        const Date&                          month,
        const std::vector<ContributionRate>& rates
    ) {
        auto contract = contractRepo.findById(contractId);
        if (!contract.has_value())
            throw std::logic_error("Contract not found: " + std::to_string(contractId));

        if (!contract->isActive())
            throw std::logic_error("Cannot create declaration for inactive contract");

        auto existing = declarationRepo.findByContractAndMonth(contractId, month);
        if (existing.has_value())
            throw std::logic_error("Declaration already exists for this contract and month");

        MonthlyDeclaration decl(
            declarationId,
            contractId,
            month,
            contract->getGrossSalary()
        );

        ContributionCalculator calc(strategyFor(contract->getType()));
        decl.setEntries(calc.calculate(declarationId, decl.getGrossBasis(), rates));

        declarationRepo.save(decl);
        return decl;
    }

    void submitDeclaration(int declarationId) {
        auto decl = declarationRepo.findById(declarationId);
        if (!decl.has_value())
            throw std::logic_error("Declaration not found: " + std::to_string(declarationId));

        decl->submit();
        declarationRepo.update(decl.value());
    }

    void acceptDeclaration(int declarationId) {
        auto decl = declarationRepo.findById(declarationId);
        if (!decl.has_value())
            throw std::logic_error("Declaration not found: " + std::to_string(declarationId));

        decl->accept();
        declarationRepo.update(decl.value());
    }

    MonthlyDeclaration getDeclaration(int declarationId) {
        auto decl = declarationRepo.findById(declarationId);
        if (!decl.has_value())
            throw std::logic_error("Declaration not found: " + std::to_string(declarationId));
        return decl.value();
    }
};