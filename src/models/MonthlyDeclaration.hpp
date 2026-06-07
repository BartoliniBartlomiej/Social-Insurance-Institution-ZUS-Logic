#pragma once

#include <vector>
#include <stdexcept>
#include "Date.hpp"
#include "ContributionEntry.hpp"

enum class DeclarationStatus {
    Draft,
    Submitted,
    Accepted,
    Corrected
};

class MonthlyDeclaration {
private:
    int               id;
    int               contractId;
    Date              declarationMonth;
    double            grossBasis;
    DeclarationStatus status;
    std::vector<ContributionEntry> entries;

public:
    MonthlyDeclaration(
        int    _id,
        int    _contractId,
        Date   _declarationMonth,
        double _grossBasis
    )
        : id(_id)
        , contractId(_contractId)
        , declarationMonth(_declarationMonth)
        , grossBasis(_grossBasis)
        , status(DeclarationStatus::Draft)
    {
        if (_grossBasis <= 0.0)
            throw std::invalid_argument("Gross basis must be positive");
    }

    int               getId()               const { return id; }
    int               getContractId()       const { return contractId; }
    const Date&       getDeclarationMonth() const { return declarationMonth; }
    double            getGrossBasis()       const { return grossBasis; }
    DeclarationStatus getStatus()           const { return status; }
    const std::vector<ContributionEntry>& getEntries() const {
        return entries;
    }

    void setEntries(std::vector<ContributionEntry> _entries) {
        if (status != DeclarationStatus::Draft)
            throw std::logic_error("Cannot modify entries on a non-draft declaration");
        if (_entries.empty())
            throw std::invalid_argument("Entries cannot be empty");
        entries = std::move(_entries);
    }

    void submit() {
        if (status != DeclarationStatus::Draft)
            throw std::logic_error("Only draft declarations can be submitted");
        if (entries.empty())
            throw std::logic_error("Cannot submit declaration without entries");
        status = DeclarationStatus::Submitted;
    }

    void accept() {
        if (status != DeclarationStatus::Submitted)
            throw std::logic_error("Only submitted declarations can be accepted");
        status = DeclarationStatus::Accepted;
    }

    void correct() {
        if (status != DeclarationStatus::Accepted)
            throw std::logic_error("Only accepted declarations can be corrected");
        status = DeclarationStatus::Corrected;
    }

    double totalInsured() const {
        double sum = 0.0;
        for (const auto& e : entries)
            sum += e.getAmountInsured();
        return sum;
    }

    double totalPayer() const {
        double sum = 0.0;
        for (const auto& e : entries)
            sum += e.getAmountPayer();
        return sum;
    }

    double totalAmount() const {
        return totalInsured() + totalPayer();
    }
};