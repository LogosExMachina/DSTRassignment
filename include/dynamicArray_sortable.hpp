#pragma once
#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <iostream>
#include <sstream>
#include <string>

//#include <vector> // REMOVE THIS !!

#include "dynamicArray_unsortable.hpp"

// this is to make sure that the Date::operator<< 
// overload reference is found! 
#include "date.hpp"

// 5 items at least
#define INITIAL_ARRAY_SIZE 5

// Function to swap any two elements 
template <typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
class DynamicArray_Sortable : public DynamicArray_Unsortable<T> {
public:

    // Set to INITIAL_ARRAY_SIZE by default
    DynamicArray_Sortable() {
        data = new T[INITIAL_ARRAY_SIZE];
        memorySize = INITIAL_ARRAY_SIZE;
        
        // Set all data to 0 by default
        for(int i=0;i<memorySize;i++)
            data[i] = 0;
    }

    // Size must be > 0. Else, the array 
    // will not be initialized
    DynamicArray_Sortable(int size) {
        if(size > 0){
            data = new T[size]; 
            memorySize=size;
        } 
        else {
            data = new T[INITIAL_ARRAY_SIZE];
            memorySize=INITIAL_ARRAY_SIZE;
        }

        // Set all data to 0 by default
        for(int i=0;i<memorySize;i++)
            data[i] = 0;
    }

    // Deallocation
    ~DynamicArray_Sortable() {
        if(autoDealloc) this->free();
    }
    
    void free() {
        std::cout << "> Destroying Dynamic Array" << std::endl;
        if(NULL != data) {
            delete[] data;
        }
    }

    bool wasInitialized() {
        return  
            NULL != data &&
            0 < memorySize &&
            0 <= virtualSize;
    }

    T popBack() {
        if(!wasInitialized()) return 0;

        // This should be replaced by some
        // sort of exception throwing
        if(virtualSize==0) return -1;

        // Create a copy of the last item
        T output = data[virtualSize-1];
        
        // Set the item to 0
        data[virtualSize-1] = 0;

        // Update virtual size
        virtualSize--;
        
        return output;
    }
    
    bool pushBack(T newItem) {
        if(!wasInitialized()) return false;

        // Check if resizing is needed
        if(virtualSize >= (memorySize-1)) {

            // Extend the real array size
            T* newBuffer = new T[memorySize+INITIAL_ARRAY_SIZE];
            // Update memory size !
            memorySize+=INITIAL_ARRAY_SIZE;

            // Copy the items to the new buffer
            for(int i=0; i<virtualSize; i++) {
                newBuffer[i] = data[i];
            }
            
            delete[] data; // Deallocate the previous data buffer 
            data = newBuffer; // Set the new buffer as current data
        }
        
        // add to the last item
        data[virtualSize] = newItem; 
        virtualSize++;
        return true;
    }

    bool insertAt(T newItem, int index) {        
        if (!wasInitialized()) return false;

        // Check out of bounds
        if(index<0 || index > (virtualSize-1)) 
            return false;

        // Modify the data
        data[index] = newItem;
        
        return true;
    }
    
    T getAt(int index) {
        if (!wasInitialized()) return NULL;

        // Check out of bounds
        if(index<0 || index > (virtualSize-1)) 
            return NULL;

        return data[index];
    }

    bool deleteAt(int index) {
        if(!wasInitialized()) return false;

        // Check out of bounds
        if(index<0 || index > (virtualSize-1)) 
            return false;

        // Reset the target item value
        data[index] = 0;

        // Translate all the elements one place
        // to the left
        for(int i=index+1; i<virtualSize; i++) {
            data[i-1]=data[i];
        }

        // Make sure the rightmost item is reset too
        data[virtualSize-1] = 0;

        // Update the size
        virtualSize--; 

        return true;
    }

    // Returns the elements of the string as 
    // an array
    std::string getAsString() {
        if (!wasInitialized()) 
            return std::string("[ NOT_INIT ]");

        std::stringstream sstream;

        sstream << "[";
        for(int i=0; i<virtualSize; i++) {
            sstream << " ";
            if(!data[i])
                sstream << "NULL_or_0"; 
            else {
                T i_data = data[i]; 
                sstream << i_data;
            }

            if(i<virtualSize-1)
                sstream << ", ";  
        }
        sstream << " ]";
        return sstream.str();
    }

    // Returns the full memory representation
    // as a string
    std::string getFullMemoryPrint() {
        if (!wasInitialized()) 
            return std::string("[ NOT_INIT ]");
        
        std::stringstream sstream;

        sstream << "[";
        for(int i=0; i<memorySize; i++) {
            sstream << " ";
            if(!data[i])
                sstream << "NULL"; 
            else {
                T i_data = data[i]; 
                sstream << i_data;
            }
                
            if(virtualSize-1==i)
                sstream << " || ";
            else {
                if(i<memorySize-1)
                    sstream << ", ";
            }
        }
        sstream << " ]";
        return sstream.str();
    }

    int getSize() {
        if(!wasInitialized()) return 0;
        return this->virtualSize;
    }

    void setVerbose(bool verbose) {
        this->verbose=verbose;
    }

    void setAutoDealloc(bool autoDealloc) {
        this->autoDealloc=autoDealloc;
    }

    // ########### SORTING FUNCTIONS ###########
    // ( Return true if the sorting
    // process was successful ) 

    bool quicksort(int low, int high){
        if (low < high) {
            // Partition the array and get the pivot index
            int pivotIndex = quicksortPartition(data, low, high);
    
            // Recursively sort the left and right subarrays
            quicksort(low, pivotIndex - 1);
            quicksort(pivotIndex + 1, high);
        } 
        
        return true;
    }

    // IMPORTANT NOTE: When passing parallelArray, always pass
    // it BY REFERENCE (Using &), NEVER BY COPY (Without the &).
    // When passing it by reference, the destructor will be called each 
    // time the quicksort recursive function returns, hence messing up the 
    // allocated data

    // parallelArray -> used to sort another array with the
    // same number of elements in the same order as the currently 
    // sorted array.  
    template<typename P> 
    bool quicksort(int low, int high, DynamicArray_Sortable<P> &parallelArray){
        if(!parallelArray.wasInitialized()) return false;

        if (low < high) {
            // Partition the array and get the pivot index
            int pivotIndex = quicksortPartition<P>(data, low, high, parallelArray);
            
            // Recursively sort the left and right subarrays
            quicksort<P>(low, pivotIndex - 1, parallelArray);
            quicksort<P>(pivotIndex + 1, high, parallelArray);
        } 
        
        return true;
    }

    // parallelArray -> used to sort another array with the
    // same number of elements in the same order as the currently 
    // sorted array.  
    template<typename P> 
    bool quicksort(int low, int high, DynamicArray_Unsortable<P> &parallelArray){
        if(!parallelArray.wasInitialized()) return false;

        if (low < high) {
            // Partition the array and get the pivot index
            int pivotIndex = quicksortPartition<P>(data, low, high, parallelArray);
            
            // Recursively sort the left and right subarrays
            quicksort<P>(low, pivotIndex - 1, parallelArray);
            quicksort<P>(pivotIndex + 1, high, parallelArray);
        } 
        
        return true;
    }

    // parallelArrays -> used to sort other arrays with the
    // same number of elements in the same order as the currently 
    // sorted array.  
    template<typename P> 
    bool quicksort(int low, int high, DynamicArray_Sortable<P>* &parallelArrays){
        if(!parallelArrays) return false;
        
        if (low < high) {
            // Partition the array and get the pivot index
            int pivotIndex = quicksortPartition<P>(data, low, high, parallelArrays);

            // Recursively sort the left and right subarrays
            quicksort<P>(low, pivotIndex - 1, parallelArrays);
            quicksort<P>(pivotIndex + 1, high, parallelArrays);
        }
        
        return true;
    }

    // Function to swap two elements from another (external) DynamicArray
    void externalSwap(int indexA, int indexB) {
        T& a = data[indexA];
        T& b = data[indexB];
        internalSwap(a, b);
    }

    bool bubblesort() {
        
    }

    // ########### SEARCH FUNCTIONS ###########
    // ( Return -1 if the sorting
    // process was unsuccessful, else returns the 
    // index of the target item) 

    // Naive, but use it in an emergency...
    // Returns the index of the first element matching the target
    int linearSearch(T elem) {
        int targetID = -1;
        for(int i=0; i<virtualSize; i++) {
            if(elem==data[i]) {
                targetID=i;
                break; 
            }
        }
        return targetID;
    }

private:
    T *data = 0;
    int memorySize=0; // The actual size in memory
    int virtualSize=0; // The size in items

    // Use this to display extra info
    bool verbose=false;

    bool autoDealloc=false;

    // ########## SORTING UTILITY FUNCTIONS ##########

    // Function to swap two elements within the same DynamicArray
    void internalSwap(T &a, T &b) {
        T temp = a;
        a = b;
        b = temp;

        // DEBUG
        if(verbose)
            std::cout << "\t>> Swapped " << a << " by " << b << std::endl;
    }

    // Partition function: Places the pivot in the correct position
    int quicksortPartition(T* arr, int low, int high) {
        T pivot = arr[high];  // Choose the last element as pivot
        int i = low - 1;        // Index of smaller element

        for (int j = low; j < high; j++) {
            // If current element is smaller than the pivot
            if (arr[j] < pivot) {
                i++;
                internalSwap(arr[i], arr[j]);  // Swap with the smaller element's position
            }
        }
        internalSwap(arr[i + 1], arr[high]);  // Place pivot in the correct position
        return i + 1;  // Return the pivot index
    }
    
    // Partition function: Places the pivot in the correct position.
    // parallelArray -> used to sort another array with the
    // same number of elements in the same order as the currently 
    // sorted array. Set it to nullptr to avoid parallel sorting.  
    template<typename P>
    int quicksortPartition(T* arr, int low, int high, DynamicArray_Sortable<P> &parallelArray){
        T pivot = arr[high];  // Choose the last element as pivot
        int i = low - 1;        // Index of smaller element

        for (int j = low; j < high; j++) {
            // If current element is smaller than the pivot
            if (arr[j] < pivot) {
                i++;
                internalSwap(arr[i], arr[j]);  // Swap with the smaller element's position
                parallelArray.externalSwap(i, j); // Do the same for the parallel array
            }
        }
        internalSwap(arr[i + 1], arr[high]);  // Place pivot in the correct position 
        parallelArray.externalSwap(i+1, high); // Do the same for the parallel array
        return i + 1;  // Return the pivot index
    }

    // Partition function: Places the pivot in the correct position.
    // parallelArray -> used to sort another array with the
    // same number of elements in the same order as the currently 
    // sorted array. Set it to nullptr to avoid parallel sorting.  
    template<typename P>
    int quicksortPartition(T* arr, int low, int high, DynamicArray_Unsortable<P> &parallelArray){
        T pivot = arr[high];  // Choose the last element as pivot
        int i = low - 1;        // Index of smaller element

        for (int j = low; j < high; j++) {
            // If current element is smaller than the pivot
            if (arr[j] < pivot) {
                i++;
                internalSwap(arr[i], arr[j]);  // Swap with the smaller element's position
                parallelArray.externalSwap(i, j); // Do the same for the parallel array
            }
        }
        internalSwap(arr[i + 1], arr[high]);  // Place pivot in the correct position 
        parallelArray.externalSwap(i+1, high); // Do the same for the parallel array
        return i + 1;  // Return the pivot index
    }

    // Partition function: Places the pivot in the correct position.
    // parallelArrays -> used to sort other arrays with the
    // same number of elements in the same order as the currently 
    // sorted array.  
    template<typename P>
    int quicksortPartition(T* arr, int low, int high, DynamicArray_Sortable<P>* &parallelArrays, int nParallels){
        T pivot = arr[high];    // Choose the last element as pivot
        int i = low - 1;        // Index of smaller element

        for (int j = low; j < high; j++) {
            // If current element is smaller than the pivot
            if (arr[j] < pivot) {
                i++;
                internalSwap(arr[i], arr[j]);  // Swap with the smaller element's position
         
                // Do the same for the parallel arrays
                for(int i=0 ; i<nParallels; i++) {
                    parallelArrays[i].externalSwap(i, j);
                }
            }
        }
        internalSwap(arr[i + 1], arr[high]);  // Place pivot in the correct position 
        
        // Do the same for the parallel arrays
        for(int i=0 ; i<nParallels; i++) {   
            parallelArrays.externalSwap(i+1, high); 
        }
        
        return i + 1;  // Return the pivot index
    }
};

#endif // DYNAMIC_ARRAY_HPP