#pragma once

#include <string>


class Person {
protected:
    std::string name;
    std::string surname;

public:
    virtual void getName() = 0;
};