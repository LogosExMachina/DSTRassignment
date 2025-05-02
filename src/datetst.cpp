#include <iostream>
#include "date.hpp"

int main(int argc, char** argv) {

    Date d1, d2;

    d1 = 10;    
    d2 = stdStringToDate(std::string("01/03/2025"));
    
    std::cout << "Date 1 = " << d1 << std::endl;//<< d1.getAsString() << std::endl;
    std::cout << "Date 2 = " << d2 << std::endl;//<< d2.getAsString() << std::endl;

    std::cout << "Is Date 1 valid ? -> " << ((!!d1)? "YES" : "NO") << std::endl;
    std::cout << "Is Date 2 valid ? -> " << ((!!d2)? "YES" : "NO") << std::endl;

    std::cout << "Is (Date 1 > Date 2) ? -> " 
        << ((d1>d2)? "YES" : "NO")
            << std::endl;

    std::cout << "Is (Date 1 < Date 2) ? -> " 
        << ((d1<d2)? "YES" : "NO")
            << std::endl;

    std::cout << "Is (Date 1 == Date 2) ? -> "
        << ((d1==d2)? "YES" : "NO")
            << std::endl;

    return 0;
}


