#pragma once
#ifndef DATE_H
#define DATE_H

#include <string>
#include <iosfwd>
#include "strUtils.h"

struct Date {
    int day; // Depends on the month
    int month; // Must be 1 - 12 
    int year;

    // Assignment overloading for an integer
    Date& operator=(const int& integer);

    // Overloading the logical NOT operator
    bool operator!() const;
    
    std::string getAsString() const;
};

bool isLeapYear(int year); 

bool isDateValid(const Date &date); 

bool operator<(const Date &date1, const Date &date2);

bool operator>(const Date &date1, const Date &date2); 

bool operator==(const Date &date1, const Date &date2);

std::ostream& operator<<(std::ostream& os, const Date &date);

//std::stringstream& operator<<(std::stringstream& ss, const Date &date);

// Reads a string 'str'  storing a Date in the dd/mm/yyyy format,
// converts it to a Date struct.
// If an error occurred, data fields are set to -1.
Date stdStringToDate(std::string str); 

#endif // DATE_H