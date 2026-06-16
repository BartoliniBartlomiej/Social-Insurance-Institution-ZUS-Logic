#pragma once

#include <stdexcept>
#include <optional>
#include "../models/Citizen.hpp"
#include "../repositories/ICitizenRepository.hpp"
#include "../validators/PeselValidator.hpp"

class CitizenService {
private:
    ICitizenRepository& repo;

public:
    explicit CitizenService(ICitizenRepository& _repo)
        : repo(_repo) {}

    void registerCitizen(const Citizen& citizen) {
        if (!PeselValidator::isValid(citizen.getPesel()))
            throw std::invalid_argument("Invalid PESEL: " + citizen.getPesel());

        auto existing = repo.findByPesel(citizen.getPesel());
        if (existing.has_value())
            throw std::logic_error("Citizen with this PESEL already exists");

        repo.save(citizen);
    }

    Citizen getCitizen(int id) {
        auto citizen = repo.findById(id);
        if (!citizen.has_value())
            throw std::logic_error("Citizen not found: " + std::to_string(id));
        return citizen.value();
    }

    Citizen getCitizenByPesel(const std::string& pesel) {
        if (!PeselValidator::isValid(pesel))
            throw std::invalid_argument("Invalid PESEL: " + pesel);

        auto citizen = repo.findByPesel(pesel);
        if (!citizen.has_value())
            throw std::logic_error("Citizen not found for PESEL: " + pesel);
        return citizen.value();
    }

    void updateAddress(int id, const Address& newAddress) {
        auto citizen = repo.findById(id);
        if (!citizen.has_value())
            throw std::logic_error("Citizen not found: " + std::to_string(id));

        citizen->updateAddress(newAddress);
        repo.update(citizen.value());
    }
};