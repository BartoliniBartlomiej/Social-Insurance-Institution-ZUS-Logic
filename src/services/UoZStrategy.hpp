#pragma once

#include "ICalculationStrategy.hpp"

class UoZStrategy : public ICalculationStrategy {
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
                    // Obowiązkowe dla UoZ
                    amountInsured = grossBasis * (rate.getRateInsured() / 100.0);
                    amountPayer   = grossBasis * (rate.getRatePayer()   / 100.0);
                    break;

                case ContributionType::Sickness:
                    // Dobrowolna dla UoZ — domyślnie włączona
                    amountInsured = grossBasis * (rate.getRateInsured() / 100.0);
                    amountPayer   = 0.0;
                    break;

                case ContributionType::HealthInsurance:
                    amountInsured = grossBasis * (rate.getRateInsured() / 100.0);
                    amountPayer   = 0.0;
                    break;

                case ContributionType::Accident:
                    // Tylko pracodawca, obowiązkowa
                    amountInsured = 0.0;
                    amountPayer   = grossBasis * (rate.getRatePayer() / 100.0);
                    break;

                case ContributionType::LaborFund:
                    // Brak dla UoZ
                    amountInsured = 0.0;
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