#pragma once

#include <vector>
#include "../models/ContributionEntry.hpp"
#include "../models/ContributionRate.hpp"

class ICalculationStrategy {
public:
    virtual ~ICalculationStrategy() = default;

    virtual std::vector<ContributionEntry> calculate(
        int declarationId, 
        double grossBasis, 
        const std::vector<ContributionRate>& rates) const = 0;
};