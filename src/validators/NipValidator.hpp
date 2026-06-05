#pragma once

#include <string>
#include <cctype>

class NipValidator {
public:
    static bool isValid(const std::string& nip) {
        if (nip.length() != 10)
            return false;

        for (unsigned char c : nip) {
            if (!std::isdigit(c))
                return false;
        }

        const int weights[] = {6, 5, 7, 2, 3, 4, 5, 6, 7};

        int sum = 0;
        for (int i = 0; i < 9; ++i) {
            sum += (nip[i] - '0') * weights[i];
        }

        int checkDigit = sum % 11;
        if (checkDigit == 10)
            return false;

        return checkDigit == (nip[9] - '0');
    }
};