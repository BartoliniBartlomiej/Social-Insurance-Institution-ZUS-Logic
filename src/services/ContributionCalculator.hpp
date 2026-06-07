#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include "ICalculationStrategy.hpp"
#include "../models/ContributionEntry.hpp"
#include "../models/ContributionRate.hpp"

class ContributionCalculator {
private:
    std::shared_ptr<ICalculationStrategy> strategy;

public:
    explicit ContributionCalculator(std::shared_ptr<ICalculationStrategy> _strategy)
        : strategy(std::move(_strategy))
    {
        if (!strategy)
            throw std::invalid_argument("Strategy cannot be null");
    }

    void setStrategy(std::shared_ptr<ICalculationStrategy> newStrategy) {
        if (!newStrategy)
            throw std::invalid_argument("Strategy cannot be null");
        strategy = std::move(newStrategy);
    }

    std::vector<ContributionEntry> calculate(
        int declarationId,
        double grossBasis,
        const std::vector<ContributionRate>& rates
    ) const {
        if (grossBasis <= 0.0)
            throw std::invalid_argument("Gross basis must be positive");
        if (rates.empty())
            throw std::invalid_argument("Rates cannot be empty");

        return strategy->calculate(declarationId, grossBasis, rates);
    }
};