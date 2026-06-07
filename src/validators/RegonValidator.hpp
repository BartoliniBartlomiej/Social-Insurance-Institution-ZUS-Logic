#pragma once

#include <string>
#include <cctype>

class RegonValidator {
public:
    static bool isValid(const std::string& regon) {
        if (regon.length() != 9 && regon.length() != 14)
            return false;

        for (unsigned char c : regon) {
            if (!std::isdigit(c))
                return false;
        }

        if (regon.length() == 9)
            return isValid9(regon);
        else
            return isValid14(regon);
    }

private:
    static bool isValid9(const std::string& regon) {
        const int weights[] = {8, 9, 2, 3, 4, 5, 6, 7};

        int sum = 0;
        for (int i = 0; i < 8; ++i)
            sum += (regon[i] - '0') * weights[i];

        int checkDigit = sum % 11;
        if (checkDigit == 10)
            checkDigit = 0;

        return checkDigit == (regon[8] - '0');
    }

    static bool isValid14(const std::string& regon) {
        const int weights[] = {2, 4, 8, 5, 0, 9, 7, 3, 6, 1, 2, 4, 8};

        int sum = 0;
        for (int i = 0; i < 13; ++i)
            sum += (regon[i] - '0') * weights[i];

        int checkDigit = sum % 11;
        if (checkDigit == 10)
            checkDigit = 0;

        return checkDigit == (regon[13] - '0');
    }
};