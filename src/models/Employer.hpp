#pragma once

#include <string>
#include <stdexcept>
#include "Person.hpp"
#include "Address.hpp"

enum class AccidentRiskCategory {
    Low,    // 0.67%
    Medium, // 1.67%
    High    // 3.33%
};

class Employer : public Person {
private:
    int id;
    std::string nip;
    std::string regon;
    AccidentRiskCategory riskCategory;
    Address address;

public:
    Employer(
        int                  _id,
        std::string          _name,
        std::string          _nip,
        std::string          _regon,
        AccidentRiskCategory _riskCategory,
        Address              _address
    )
        : Person(std::move(_name), "")
        , id(_id)
        , nip(std::move(_nip))
        , regon(std::move(_regon))
        , riskCategory(_riskCategory)
        , address(std::move(_address))
    {
        if (nip.empty())
            throw std::invalid_argument("NIP cannot be empty");
        if (regon.empty())
            throw std::invalid_argument("REGON cannot be empty");
    }

    int                        getId()             const { return id; }
    const std::string&         getNip()            const { return nip; }
    const std::string&         getRegon()          const { return regon; }
    AccidentRiskCategory       getRiskCategory()   const { return riskCategory; }
    const Address&             getAddress()        const { return address; }

    std::string getFullName() const override {
        return name;
    }

    void updateAddress(Address newAddress) {
        address = std::move(newAddress);
    }

    void updateRiskCategory(AccidentRiskCategory newCategory) {
        riskCategory = newCategory;
    }
};