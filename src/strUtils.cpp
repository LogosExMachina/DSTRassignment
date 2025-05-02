#include "strUtils.h"

// Overload this, later needed by other components
bool operator!(const std::string& str) {
    return str.empty();
}

// Overload this, later needed by other components
// like DynamicArray_Unsortable
bool operator==(const std::string&str, const std::nullptr_t& nptr) {
    return str.empty();
}

// Overload this, later needed by other components
// like DynamicArray_Unsortable
bool operator==(const std::nullptr_t& nptr, const std::string&str) {
    return str.empty();
}

// Erases the leading whitespace from 'str'
// Warning: this will modify 'str'
void trimLeadingInPlace_STL(std::string &str) {
    str.erase(
        str.begin(),
        std::find_if(
            str.begin(),
            str.end(),
            [](unsigned char ch) {
                return !std::isspace(ch);
            }
        )
    );
}

// Erases the trailing whitespace from 'str'
// Warning: this will modify 'str'
void trimTrailingInPlace_STL(std::string &str) {
    str.erase(
        std::find_if(
            str.rbegin(),
            str.rend(),
            [](unsigned char ch) {
                return !std::isspace(ch);
            }
        ).base(),
        str.end()
    );
}

/*
// Returns a new string without the leading (front) whitespace
std::string trimLeading(std::string str) {

    std::stringstream ssbuffer("");

    // 'Swap' count between a whitespace
    // and any other char
    int swap=0;

    for(auto i=str.begin(); i!=str.end(); i++) {

        // Do not check this the the beginning since
        // we are doing i-1
        if(i!=str.begin())
            if(
                // char i is whitespace AND previous
                // char is not a whitespace
                ((' ' == *i) && (' ' != *(i-1))) || // OR
                // char i is not whitespace AND previous
                // char is a whitespace
                ((' ' != *i) && (' ' == *(i-1)))
            ) swap++;

        // Break before arriving to the trailing whitespace
        if(swap>=2) break;

        // Add character if its not a whitespace
        if(' ' != *i) ssbuffer << *i;
    }

    return ssbuffer.str();
}
*/

/*
// Returns a new string without the trailing (back) whitespace
std::string trimTrailing(std::string str) {

    std::stringstream ssbuffer("");

    // 'Swap' count between a whitespace
    // and any other char
    int swap=0;

    for(auto i=str.end()-1; i>=str.begin(); i--) {
        // Do not check this the beginning (end()) since
        // we are doing i+1
        if(i!=str.end()-1)
        if(
            // char i is whitespace AND previous
            // char is not a whitespace
            ((' ' == *i) && (' ' != *(i+1))) ||
            // char i is not whitespace AND previous
            // char is a whitespace
            ((' ' != *i) && (' ' == *(i+1)))
        ) swap++;

            // Break before arriving to the leading whitespace
            if(swap>=2) break;

            // Add character if its not a whitespace
            if(' ' != *i) ssbuffer << *i;
        }

        std::string reversed = ssbuffer.str();
        ssbuffer = std::stringstream();
        for(auto i=reversed.end()--; i<=reversed.begin(); i++)
        ssbuffer << *i;

        std::string out = ssbuffer.str();
        return out;
}

*/
