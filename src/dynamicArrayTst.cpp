#include "dynamicArray/dynamicArray_sortable.hpp"

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

    //arr.quicksort(0, arr.getSize()-1);
    //arr.mergesort();
    //arr.bucketSort(2);
    arr.insertionSort();

    arrStr = arr.getAsString();
    std::cout <<  "Sorted array: " << arrStr << std::endl;

    int index = arr.interpolationSearch(4);
    //arr.jumpSearch(4); 
    //arr.binarySearch(4);
    std::cout << "Index of '4' = " << index << std::endl;
    
    //std::string fullArrStr = arr.getFullMemoryPrint();
    //std::cout << arrStr << std::endl;
    //std::cout << fullArrStr << std::endl;

    return 0;
}
