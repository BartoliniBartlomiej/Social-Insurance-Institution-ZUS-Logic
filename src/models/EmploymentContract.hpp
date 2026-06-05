#pragma once

#include <string>

#include "Date.hpp"

enum class ContractType {
    UoP,
    UoZ,
    B2B
};

enum class ContractStatus {
    Active,
    Terminated,
    Expired
};

class EmploymentContract {
private:
    int            id;
    int            citizenId;
    int            employerId;
    ContractType   type;
    double         grossSalary;
    Date           startDate;
    Date           endDate;
    ContractStatus status;

public:
    EmploymentContract(
        int          _id,
        int          _citizenId,
        int          _employerId,
        ContractType _type,
        double       _grossSalary,
        Date         _startDate,
        Date         _endDate
    )
        : id(_id)
        , citizenId(_citizenId)
        , employerId(_employerId)
        , type(_type)
        , grossSalary(_grossSalary)
        , startDate(_startDate)
        , endDate(_endDate)
        , status(ContractStatus::Active)
    {
        if (grossSalary <= 0)
            throw std::invalid_argument("Gross salary must be positive");
        if (endDate <= startDate)
            throw std::invalid_argument("End date must be after start date");
    }

    int            getId()          const { return id; }
    int            getCitizenId()   const { return citizenId; }
    int            getEmployerId()  const { return employerId; }
    ContractType   getType()        const { return type; }
    double         getGrossSalary() const { return grossSalary; }
    const Date&    getStartDate()   const { return startDate; }
    const Date&    getEndDate()     const { return endDate; }
    ContractStatus getStatus()      const { return status; }

    bool isActive() const {
        return status == ContractStatus::Active;
    }

    void terminate(const Date& terminationDate) {
        if (!isActive())
            throw std::logic_error("Cannot terminate a contract that is not active");
        if (terminationDate < startDate)
            throw std::invalid_argument("Termination date cannot be before start date");

        endDate = terminationDate;
        status = ContractStatus::Terminated;
    }

    void expire() {
        if (!isActive())
            throw std::logic_error("Cannot expire a contract that is not active")
    }
};