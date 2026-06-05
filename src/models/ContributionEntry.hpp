#pragma once

#include <stdexcept>
#include "ContributionRate.hpp"

class ContributionEntry {
private:
    int              id;
    int              declarationId;
    int              rateId;
    ContributionType type;
    double           amountInsured;
    double           amountPayer;

public:
    ContributionEntry(
        int              _id,
        int              _declarationId,
        int              _rateId,
        ContributionType _type,
        double           _amountInsured,
        double           _amountPayer
    )
        : id(_id)
        , declarationId(_declarationId)
        , rateId(_rateId)
        , type(_type)
        , amountInsured(_amountInsured)
        , amountPayer(_amountPayer)
    {
        if (_amountInsured < 0.0)
            throw std::invalid_argument("amountInsured cannot be negative");
        if (_amountPayer < 0.0)
            throw std::invalid_argument("amountPayer cannot be negative");
    }

    int              getId()            const { return id; }
    int              getDeclarationId() const { return declarationId; }
    int              getRateId()        const { return rateId; }
    ContributionType getType()          const { return type; }
    double           getAmountInsured() const { return amountInsured; }
    double           getAmountPayer()   const { return amountPayer; }

    double totalAmount() const {
        return amountInsured + amountPayer;
    }
};