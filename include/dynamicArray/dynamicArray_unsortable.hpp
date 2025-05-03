#pragma once
#ifndef DYNAMIC_ARRAY_UNSORT_HPP
#define DYNAMIC_ARRAY_UNSORT_HPP

#include <iostream>
#include <sstream>
#include <string>

//#include <vector> // REMOVE THIS !!

// this is to make sure that the Date::operator<< 
// overload reference is found! 
#include "shared/date.hpp"

// 5 items at least
#define INITIAL_ARRAY_SIZE 5

template <typename T>
class DynamicArray_Unsortable {
public:

    // Set to INITIAL_ARRAY_SIZE by default
    DynamicArray_Unsortable() {
        data = new T[INITIAL_ARRAY_SIZE];
        memorySize = INITIAL_ARRAY_SIZE;
        
        // Set all data to default value
        for(int i=0;i<memorySize;i++)
            data[i] = T();
    }

    // Size must be > 0. Else, the array 
    // will not be initialized
    DynamicArray_Unsortable(int size) {
        if(size > 0){
            data = new T[size]; 
            memorySize=size;
        } 
        else {
            data = new T[INITIAL_ARRAY_SIZE];
            memorySize=INITIAL_ARRAY_SIZE;
        }

        // Set all data to default value
        for(int i=0;i<memorySize;i++)
            data[i] = T();
    }

    // Deallocation
    ~DynamicArray_Unsortable() {
        if(autoDealloc) {
            this->free();
        }
    }
    
    // Warning: Deallocates the data buffer
    void free() {
        if(NULL != data) {
            std::cout << "> Deallocating Dynamic Array (name = " <<
            this->name << ")" << std::endl;
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
        data[virtualSize-1] = T();

        // Update virtual size
        virtualSize--;
        
        return output;
    }
    
    bool pushBack(T newItem) {
        if(!wasInitialized()) return false;

        if(verbose) {
            std::cout << "\t>> pushBack called with: [" << newItem << "]\n";
            std::cout << "\t   virtualSize = " << virtualSize << ", memorySize = " << memorySize << "\n";
        }

        // Check if resizing is needed
        if(virtualSize >= (memorySize-1)) {

            // Extend the real array size
            T* newBuffer = new T[memorySize+INITIAL_ARRAY_SIZE];
            // Update memory size !
            memorySize+=INITIAL_ARRAY_SIZE;

            // Copy the items to the new buffer
            for(int i=0; i<virtualSize; i++) {
                newBuffer[i] = T(data[i]);
            }
            
            delete[] data; // Deallocate the previous data buffer 
            data = newBuffer; // Set the new buffer as current data
        }
        
        // add to the last item
        data[virtualSize] = T(newItem); 
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
        if (!wasInitialized()) return T();

        // Check out of bounds
        if(index<0 || index > (virtualSize-1)) 
            return T();

        return data[index];
    }

    bool deleteAt(int index) {
        if(!wasInitialized()) return false;

        // Check out of bounds
        if(index<0 || index > (virtualSize-1)) 
            return false;

        // Reset the target item value
        data[index] = T();

        // Translate all the elements one place
        // to the left
        for(int i=index+1; i<virtualSize; i++) {
            data[i-1]=data[i];
        }

        // Make sure the rightmost item is reset too
        data[virtualSize-1] = T();

        // Update the size
        virtualSize--; 

        return true;
    }

    // Returns the elements of the string as 
    // an array
    std::string getAsString() {
        if (!wasInitialized()) 
            return std::string("[ NOT_INIT ]");

        std::string ssbuffer;
        ssbuffer.reserve(128); // avoid weird segfault
        std::stringstream sstream = std::stringstream("");
        
        sstream << "[";
        
        for(int i=0; i<virtualSize; i++) {
            sstream << " ";
            
            //if(nullptr==data[i])
            //    sstream << "NULL_or_0"; 
            //else {
            T i_data = data[i]; 
            sstream << i_data;
            //}
                
            if(i<virtualSize-1)
            sstream << ", ";  
        }
        sstream << " ]";
        
        return std::string(sstream.str());
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

    void setName(std::string name) {
        this->name=name;
    }

    // Function to swap two elements from another (external) DynamicArray
    void externalSwap(int indexA, int indexB) {
        T& a = data[indexA];
        T& b = data[indexB];
        internalSwap(a, b);
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

    // If this is true, the data is deallocated
    // through the destructor
    bool autoDealloc=true;

    // Identifier used for debugging
    std::string name="NO_NAME";

    // SORTING UTILITY FUNCTIONS

    // Function to swap two elements within the same DynamicArray
    void internalSwap(T &a, T &b) {
        T temp = a;
        a = b;
        b = temp;

        // DEBUG
        if(verbose)
            std::cout << "\t>> Swapped " << a << " by " << b << std::endl;
    }
};

#endif // DYNAMIC_ARRAY_UNSORT_HPP