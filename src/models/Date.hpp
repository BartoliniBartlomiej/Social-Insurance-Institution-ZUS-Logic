#pragma once

#include <string>
#include <stdexcept>

struct Date {
    int day;
    int month;
    int year;

    Date(int d, int m, int y) : day(d), month(m), year(y) {
        validate();
    }

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    bool operator<(const Date& other) const {
        if (year != other.year)   return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator<=(const Date& other) const {
        return *this < other || *this == other;
    }

    std::string toString() const {
        return pad(day) + "." + pad(month) + "." + std::to_string(year);
    }

private:
    void validate() const {
        if (month < 1 || month > 12)
            throw std::invalid_argument("Invalid month: " + std::to_string(month));
        if (day < 1 || day > daysInMonth())
            throw std::invalid_argument("Invalid day: " + std::to_string(day));
    }

    int daysInMonth() const {
        const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int d = days[month - 1];
        if (month == 2 && isLeapYear()) d = 29;
        return d;
    }

    bool isLeapYear() const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    std::string pad(int n) const {
        return (n < 10 ? "0" : "") + std::to_string(n);
    }
};


