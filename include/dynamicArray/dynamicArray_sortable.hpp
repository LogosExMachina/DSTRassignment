#pragma once
#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

#include "dynamicArray_unsortable.hpp"

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
        if(!wasInitialized()) return false;

        int n = virtualSize;
        for(int i=0; i< n - 1 ; i++) {
            for(int j=0; j < n - i - 1; j++) {
                T &t1 = data[j];
                T &t2 = data[j+1];
                if(t2 < t1) {
                    internalSwap(data[j], data[j+1]);
                }
            }
        }
        return true;
    }

    bool mergesort() {
        if(!wasInitialized()) return false;

        internalMergeSort(0, virtualSize-1);
        
        return true;
    }
    
    void insertionSort()
    {
        for (int i = 1; i < virtualSize; ++i) {
            int key = data[i];
            int j = i - 1;

            // Move elements of arr[0..i-1], that are
            // greater than key, to one position ahead
            // of their current position
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j = j - 1;
            }
            data[j + 1] = key;
        }
    }
    
    template<typename P>
    void insertionSort(DynamicArray_Sortable<P> &parallelArray)
    {
        for (int i = 1; i < virtualSize; ++i) {
            T key = data[i];
            P parallelKey = parallelArray.getAt(i);
            int j = i - 1;

            // Move elements of arr[0..i-1], that are
            // greater than key, to one position ahead
            // of their current position
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                P jParallelItem = parallelArray.getAt(j);
                parallelArray.insertAt(jParallelItem, j);

                j = j - 1;
            }

            data[j + 1] = key;
            parallelArray.insertAt(parallelKey, j+1);
        }
    }

    bool bucketSort(int n) {
        // 1) Create n empty buckets
        DynamicArray_Sortable<T> b[n];

        // 2) Put array elements int different buckets
        for (int i = 0; i < n; i++) {
            int bi = n * data[i];
            b[i] = DynamicArray_Sortable<T>();
            b->setAutoDealloc(false);
            b[bi].pushBack(data[i]);
        }

        // 3) Sort individual buckets using insertion sort
        for (int i = 0; i < n; i++) {
            b[i].quicksort(0, b[i].getSize()-1);
        }
        
        // 4) Concatenate all buckets into arr[]
        int index = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < b[i].getSize(); j++) {
                data[index++] = b[i].getAt(j);
            }
        }

        // 5) Deallocate buckets
        for(int i=0; i<n; i++)
            b[i].free();
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

    // Iterative approach to binary search
    // Returns the index of the first element matching the target
    // (or -1 if it was not found)
    int binarySearch(T elem) {
        if(!wasInitialized()) return -1;

        int low = 0;
        int high = virtualSize-1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
    
            // Check if x is present at mid
            if (data[mid] == elem)
                return mid;
    
            // If x greater, ignore left half
            if (data[mid] < elem)
                low = mid + 1;
    
            // If x is smaller, ignore right half
            else
                high = mid - 1;
        }
    
        // If we reach here, then element was not present
        return -1;
    }

    // Returns the index of the first element matching the target
    int jumpSearch(T elem) {
        // Finding block size to be jumped
        int step = sqrt(virtualSize);

        // Finding the block where element is
        // present (if it is present)
        int prev = 0;
        while (data[std::min(step, virtualSize)-1] < elem)
        {
            prev = step;
            step += sqrt(virtualSize);
            if (prev >= virtualSize)
                return -1;
        }
    
        // Doing a linear search for x in block
        // beginning with prev.
        while (data[prev] < elem)
        {
            prev++;
    
            // If we reached next block or end of
            // array, element is not present.
            if (prev == std::min(step, virtualSize))
                return -1;
        }
        // If element is found
        if (data[prev] == elem)
            return prev;

        // Else, not found
        return -1;  
    }

    int interpolationSearch(T elem) {
        int pos;

        int low = 0;
        int high = virtualSize-1;

        // Since array is sorted, an element present
        // in array must be in range defined by corner
        if (low <= high && elem >= data[low] && elem <= data[high]) {
            // Probing the position with keeping
            // uniform distribution in mind.
            pos = low
                + (((double)(high - low) / (data[high] - data[low]))
                    * (elem - data[low]));

            // Condition of target found
            if (data[pos] == elem)
                return pos;

            // If x is larger, x is in right sub array
            if (data[pos] < elem)
                return interpolationSearch(pos + 1, high, elem);

            // If x is smaller, x is in left sub array
            if (data[pos] > elem)
                return interpolationSearch(low, pos - 1, elem);
        }
        return -1; // Not found...
    }

    int interpolationSearch(int low, int high, T elem) {
        int pos;

        // Since array is sorted, an element present
        // in array must be in range defined by corner
        if (low <= high && elem >= data[low] && elem <= data[high]) {
            // Probing the position with keeping
            // uniform distribution in mind.
            pos = low
                + (((double)(high - low) / (data[high] - data[low]))
                    * (elem - data[low]));

            // Condition of target found
            if (data[pos] == elem)
                return pos;

            // If x is larger, x is in right sub array
            if (data[pos] < elem)
                return interpolationSearch(pos + 1, high, elem);

            // If x is smaller, x is in left sub array
            if (data[pos] > elem)
                return interpolationSearch(low, pos - 1, elem);
        }
        return -1; // Not found...
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

    void merge(int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Create temp arrays
        T* L = new T(n1);
        T* R = new T(n2);

        // Copy data to temp arrays L[] and R[]
        for (int i = 0; i < n1; i++)
            L[i] = data[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = data[mid + 1 + j];

        int i = 0, j = 0;
        int k = left;

        // Merge the temp arrays back 
        // into data [left..right]
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                data[k] = L[i];
                i++;
            }
            else {
                data[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy the remaining elements of L[], 
        // if there are any
        while (i < n1) {
            data[k] = L[i];
            i++;
            k++;
        }

        // Copy the remaining elements of R[], 
        // if there are any
        while (j < n2) {
            data[k] = R[j];
            j++;
            k++;
        }
        
        // DO NOT FORGET...
        delete[] L;
        delete[] R;
    }

    // begin is for left index and end is right index
    // of the sub-array of arr to be sorted
    void internalMergeSort(int left, int right)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;
        internalMergeSort(left, mid);
        internalMergeSort(mid + 1, right);
        merge(left, mid, right);
    }

    // ########## SEARCHING UTILITY FUNCTIONS ##########

};

#endif // DYNAMIC_ARRAY_HPP