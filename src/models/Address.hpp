#pragma once

#include <string>
#include <iostream>

class Address {
private:
    std::string street;
    std::string city;
    std::string postalCode;

public:
    Address(std::string _street, std::string _city, std::string _postalCode) :
        street(_street), city(_city), postalCode(_postalCode) {}

    
    std::string getStreet() const {   return street;  }
    std::string getCity() const {   return city;  }
    std::string getPostalCode() const {   return postalCode;  }
    std::string getFullAddress() const {    return street + ", " + city + ", " + postalCode + "\n"; }
   
};

std::ostream& operator<<(std::ostream& os, const Address& a) {
    os << a.getStreet() << ", " << a.getCity() << ", " << a.getPostalCode();
    return os;
}