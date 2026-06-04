#include <iostream>
#include "models/Person.hpp"
#include "models/Address.hpp"
#include "validators/PeselValidator.hpp"



int main() {

    std::cout << PeselValidator::isValid("02262902696");
    
    // std::cout << PeselValidator::isPESELCorrespondsToDateOfBirth("02262902696", {29, 6, 2002});
    return 0;
}