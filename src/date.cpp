#include "shared/date.hpp"

// Assignment overloading for an integer
Date& Date::operator=(const int &integer) {
    day=integer;
    month=integer;
    year=integer;
}

// Overloading the logical NOT operator
bool Date::operator!() const {
    return !isDateValid(*this);
}

std::string Date::getAsString() const {
    std::string out;
    out+=std::to_string(day);
    out+="/";
    out+=std::to_string(month);
    out+="/";
    out+=std::to_string(year);
    return out;
}

bool isLeapYear(int year)
{
    return (year % 4 == 0) &&
    ((year % 100 != 0) || (year % 400 == 0));
}

bool isDateValid(const Date &date)
{
    return
    // Validate the months
    (date.month >= 1) && (date.month <= 12) &&

    // Validate the days
    (date.day >= 1)
    &&
    ((2 == date.month)? // Is month february?
        (isLeapYear(date.year)?
            (29 >= date.day) // is leap year
            : (28 >= date.day) // is not leap year
        )
        : ( // Month is not february...
            (date.month == 4 || date.month == 6 ||
            date.month == 9 || date.month == 11) ?
                  date.day <= 30 :
                  date.day <= 31
        /*
            (0 == (date.month % 2))? // Is month even?
                (30 >= date.day) // month is even
                : (31 >= date.day) // month is odd
        */
        )
    );
}

bool operator<(const Date &date1, const Date &date2)
{
    return
    (date1.year != date2.year)? // If years are equal, compare months
    (date1.year < date2.year)
    : (
        (date1.month != date2.month)? // If months are equal, compare days
        (date1.month < date2.month)
            : (date1.day < date2.day)
    );
}

bool operator>(const Date &date1, const Date &date2)
{
    return
    (date1.year != date2.year)? // If years are equal, compare months
    (date1.year > date2.year)
    : (
        (date1.month != date2.month)? // If months are equal, compare days
        (date1.month > date2.month)
            : (date1.day > date2.day)
    );
}

bool operator==(const Date &date1, const Date &date2) {
    return (
        date1.day==date2.day &&
        date1.month==date2.month &&
        date1.year==date2.year
    );
}

std::ostream& operator<<(std::ostream& os, const Date &date) {
    os << date.getAsString();
    return os;
}

/*
std::stringstream& operator<<(std::stringstream& ss, const Date &date) {
    ss << date.getAsString();
    return ss;
}
*/

// Reads a string 'str'  storing a Date in the dd/mm/yyyy format,
// converts it to a Date struct.
// If an error occurred, data fields are set to -1.
Date stdStringToDate(std::string str)
{

    Date output;
    output.day=-1;
    output.month=-1;
    output.year=-1;

    // dd/mm/yyyy -> 10 characters at least
    if(str.size() < 10) return output;

    std::stringstream sstream;

    // 0 -> day
    // 1 -> month
    // 2 -> year
    int dataFieldCounter = 0;
    for(int i=0; i < str.size(); i++) {
        //if(dataFieldCounter>2) break;

        if('/' == str[i] || '\\' == str[i] || // Separator met
            (str.size()-1==i) // last character
        ) {
            switch(dataFieldCounter) {
                case 0:
                    output.day = atoi(sstream.str().c_str());
                    break;
                case 1:
                    output.month = atoi(sstream.str().c_str());
                    break;
                case 2:
                    output.year = atoi(sstream.str().c_str());
                    break;
                default: break;
            }
            sstream = std::stringstream(""); // reset buffer
            dataFieldCounter++;
        } else {
            sstream << str[i]; // push character to the buffer
            if (str.size()-1==i+1)
                sstream << str[i+1]; // push also the next character if its the last
        }
    }

    return output;
}
