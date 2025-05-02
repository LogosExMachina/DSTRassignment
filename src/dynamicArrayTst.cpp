#include "dynamicArray_sortable.hpp"

int main(int argc, char** argv) {  
    DynamicArray_Sortable<int> arr;
    
    arr.pushBack(1);
    arr.pushBack(4);
    arr.pushBack(12);
    arr.pushBack(2);
    arr.pushBack(13);
    arr.pushBack(3);
    
    std::string arrStr = arr.getAsString();
    std::cout << "Unsorted array: " << arrStr << std::endl;

    arr.quicksort(0, arr.getSize()-1);

    arrStr = arr.getAsString();
    std::cout <<  "Sorted array: " << arrStr << std::endl;

    //std::string fullArrStr = arr.getFullMemoryPrint(); 
    //std::cout << arrStr << std::endl;
    //std::cout << fullArrStr << std::endl;

    return 0;
}