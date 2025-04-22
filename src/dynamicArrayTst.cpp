
#include "dynamicArray.hpp"

int main(int argc, char** argv) {  
    DynamicArray<int> arr;

    arr.pushBack(1);
    arr.pushBack(2);
    arr.pushBack(3);

    arr.deleteAt(1);

    std::cout << arr.popBack() << std::endl;
    std::cout << arr.popBack() << std::endl;
    std::cout << arr.popBack() << std::endl;
    
    //std::string arrStr = arr.getAsString();
    //std::string fullArrStr = arr.getFullMemoryPrint(); 
    //std::cout << arrStr << std::endl;
    //std::cout << fullArrStr << std::endl;

    return 0;
}