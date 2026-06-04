#pragma once

#include <string>

// #include "Person.hpp"
#include "Address.hpp"
#include "Date.hpp"

class Citizen{

private:

    int id;
    std::string name;
    std::string secondName;
    std::string surname;
    std::string pesel;
    Date dateBirth;
    // std::string address;
    Address address;

public:
    Citizen(std::string _name, std::string _secondName, std::string _surname, std::string _pesel, Date _dateBairth, Address _address) :
        name(_name),
        secondName(_secondName),
        surname(_surname),
        pesel(_pesel),
        dateBirth(_dateBairth),
        address(_address) 
        {}
    
};