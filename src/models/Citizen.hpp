#pragma once

#include <string>
#include <stdexcept>
#include "Person.hpp"
#include "Address.hpp"
#include "Date.hpp"

class Citizen : public Person {
private:
    int id;
    std::string secondName;
    std::string pesel;
    Date dateBirth;
    Address address;

public:
    Citizen(
        int         _id,
        std::string _name,
        std::string _secondName,
        std::string _surname,
        std::string _pesel,
        Date        _dateBirth,
        Address     _address
    )
        : Person(std::move(_name), std::move(_surname))
        , id(_id)
        , secondName(std::move(_secondName))
        , pesel(std::move(_pesel))
        , dateBirth(_dateBirth)
        , address(std::move(_address))
    {
        if (pesel.empty())
            throw std::invalid_argument("PESEL cannot be empty");
    }

    int                getId()         const { return id; }
    const std::string& getSecondName() const { return secondName; }
    const std::string& getPesel()      const { return pesel; }
    const Date&        getDateBirth()  const { return dateBirth; }
    const Address&     getAddress()    const { return address; }


    std::string getFullName() const override {
        if (secondName.empty())
            return name + " " + surname;
        return name + " " + secondName + " " + surname;
    }

    void updateAddress(Address newAddress) {
        address = std::move(newAddress);
    }
};