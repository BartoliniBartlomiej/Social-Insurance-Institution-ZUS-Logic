#pragma once

#include <stdexcept>
#include "Date.hpp"

enum class ContributionType {
    Pension,      // Emerytalna
    Disability,   // Rentowa
    Sickness,     // Chorobowa
    Accident,     // Wypadkowa
    HealthInsurance, // Zdrowotna
    LaborFund     // Fundusz Pracy
};


class ContributionRate {
private:
    int              id;
    ContributionType type;
    double           rateInsured;  // % paid by employee
    double           ratePayer;    // % paid by employer
    Date             validFrom;
    Date             validTo;

public:
    ContributionRate(
        int              _id,
        ContributionType _type,
        double           _rateInsured,
        double           _ratePayer,
        Date             _validFrom,
        Date             _validTo
    )
        : id(_id)
        , type(_type)
        , rateInsured(_rateInsured)
        , ratePayer(_ratePayer)
        , validFrom(_validFrom)
        , validTo(_validTo)
    {
        if (_rateInsured < 0.0 || _rateInsured > 100.0)
            throw std::invalid_argument("rateInsured must be between 0 and 100");
        if (_ratePayer < 0.0 || _ratePayer > 100.0)
            throw std::invalid_argument("ratePayer must be between 0 and 100");
        if (!(_validFrom < _validTo))
            throw std::invalid_argument("validFrom must be before validTo");
    }

    int              getId()          const { return id; }
    ContributionType getType()        const { return type; }
    double           getRateInsured() const { return rateInsured; }
    double           getRatePayer()   const { return ratePayer; }
    const Date&      getValidFrom()   const { return validFrom; }
    const Date&      getValidTo()     const { return validTo; }

    bool isActiveFor(const Date& date) const {
        return validFrom <= date && date <= validTo;
    }

    double totalRate() const {
        return rateInsured + ratePayer;
    }
};