#pragma once

#include "ICalculationStrategy.hpp"

class B2BStrategy : public ICalculationStrategy {
public:
    std::vector<ContributionEntry> calculate(
        int declarationId,
        double grossBasis,
        const std::vector<ContributionRate>& rates
    ) const override {
        std::vector<ContributionEntry> entries;

        int entryId = 1;
        for (const auto& rate : rates) {
            double amountInsured = 0.0;
            double amountPayer   = 0.0;

            switch (rate.getType()) {
                case ContributionType::Pension:
                case ContributionType::Disability:
                case ContributionType::Accident:
                case ContributionType::LaborFund:
                    amountInsured = grossBasis * (rate.totalRate() / 100.0);
                    amountPayer   = 0.0;
                    break;

                case ContributionType::Sickness:
                    amountInsured = grossBasis * (rate.getRateInsured() / 100.0);
                    amountPayer   = 0.0;
                    break;

                case ContributionType::HealthInsurance:
                    amountInsured = grossBasis * (rate.getRateInsured() / 100.0);
                    amountPayer   = 0.0;
                    break;
            }

            entries.emplace_back(
                entryId++,
                declarationId,
                rate.getId(),
                rate.getType(),
                amountInsured,
                amountPayer
            );
        }

        return entries;
    }
};