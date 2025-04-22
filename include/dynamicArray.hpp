#include <iostream>
#include <string>
#include <sstream>

// 5 items at least
#define INITIAL_ARRAY_SIZE 5

template <typename T>
class DynamicArray {
public:

    // Set to INITIAL_ARRAY_SIZE by default
    DynamicArray() {
        data = new T[INITIAL_ARRAY_SIZE];
        memorySize = INITIAL_ARRAY_SIZE;
        
        // Set all data to 0 by default
        for(int i=0;i<memorySize;i++)
            data[i] = 0;
    }

    // Size must be > 0. Else, the array 
    // will not be initialized
    DynamicArray(int size) {
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
    ~DynamicArray() {
        if(NULL != data) {
            delete data;
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
            
            // Copy the items to the new buffer
            for(int i=0; i<virtualSize; i++) {
                newBuffer[i] = data[i];
            }
            
            delete data; // Deallocate the previous data buffer 
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
            else
                sstream << data[i];

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
            else
                sstream << data[i];
    
                
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

private:
    T *data = 0;
    int memorySize=0; // The actual size in memory
    int virtualSize=0; // The size in items
};
