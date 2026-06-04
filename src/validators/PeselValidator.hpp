#pragma once
#include <string>
#include <cctype>
#include "../models/Date.hpp"

int getMonth(int month)
{
    return month % 20 == 0 ? 12 : month % 20;
}

class PeselValidator {
public:
    static bool isValid(const std::string& pesel) {
        if (pesel.length() != 11)
            return false;

        for (unsigned char c : pesel) {
            if (!std::isdigit(c))
                return false;
        }

        const int wagi[] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3};

        int sum = 0;
        for (int i = 0; i < 10; ++i) {
            sum += (pesel[i] - '0') * wagi[i];
        }

        int kontrolna = (10 - (sum % 10)) % 10;

        return kontrolna == (pesel[10] - '0');
    }


    static bool isPESELCorrespondsToDateOfBirth(const std::string& pesel, const Date& date) {
        if (!isValid(pesel))
            return false;

        int mm = (pesel[2] - '0') * 10 + (pesel[3] - '0');

        int yearPESEL = (pesel[0] - '0') * 10 + (pesel[1] - '0');

        if (mm <= 12) yearPESEL += 1900;
        else if (mm <= 32) yearPESEL += 2000;
        else if (mm <= 52) yearPESEL += 2100;
        else if (mm <= 72) yearPESEL += 2200;
        else if (mm <= 92) yearPESEL += 1800;

        int monthPESEL = mm % 20;

        int dayPESEL = (pesel[4] - '0') * 10 + (pesel[5] - '0');

        return (date.year == yearPESEL &&
                date.month == monthPESEL &&
                date.day == dayPESEL);
    }
};