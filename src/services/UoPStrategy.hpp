#pragma once

#include "ICalculationStrategy.hpp"

class UoPStrategy : public ICalculationStrategy {
public:
    virtual std::vector<ContributionEntry> calculate(
        int declarationId,
        double grossBasis,
        const std::vector<ContributionRate>& rates
    ) const override {
        std::vector<ContributionEntry> entries;

        int entryId = 1;
        for (const auto& rate : rates) {
            double amountInsured = 0;
            double amountPayer = 0;

            switch (rate.getType()) {
                case ContributionType::Pension:
                case ContributionType::Disability:
                case ContributionType::Sickness:
                case ContributionType::HealthInsurance:
                    amountInsured = grossBasis * (rate.getRateInsured() / 100.0);
                    amountPayer = grossBasis * (rate.getRatePayer() / 100.0);
                    break;
                
                case ContributionType::Accident:
                case ContributionType::LaborFund:
                    amountInsured = 0.0;
                    amountPayer = grossBasis * (rate.getRatePayer() / 100.0);
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