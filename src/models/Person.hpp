#pragma once

#include <string>

class Person {
protected:
    std::string name;
    std::string surname;

public:
    Person(std::string _name, std::string _surname)
        : name(std::move(_name)), surname(std::move(_surname)) {}

    virtual ~Person() = default;

    virtual std::string getFullName() const = 0;

    std::string getName() const { return name; }
    std::string getSurname() const { return surname; }
};