#include <iostream>
#include "dataTable.h"
#include "csvParser.h"
#include "dynamicArray.hpp"
#include "date.hpp"

// Solution 1: How can you efficiently sort customer transactions by date
// and display the total number of transactions in both datasets? 

CSVParser parser;
DataTable rawTransactions, sortedTransactions;
const int DATE_COLUMN_INDEX = 4;
const int CUSTID_COLUMN_INDEX = 0;
const char *CUSTID_PREFIX = "CUST";

// SETUP 1: Load the CSV data into a DataTable
void loadTransactions() {

    ColumnType columnTypes[] = {
        ColumnType::STRING, // Customer ID
        ColumnType::STRING, // Product
        ColumnType::STRING, // Category
        ColumnType::DOUBLE, // Price
        ColumnType::STRING, // Date 
        ColumnType::STRING  // Payment Method 
    };
    
    int stringLengths[] = {
        16, // Customer ID
        32, // Product
        16, // Category
        -1, // Price
        16, // Date 
        32 // Payment Method
    };
    
    int nCols = sizeof(columnTypes)/sizeof(ColumnType);
    
    rawTransactions
    = parser.parseCSV(
        "data\\transactions_cleaned.csv", 
        //"data\\reviews_cleaned.csv",
        columnTypes,
        stringLengths,
        nCols,
        //10
        -1 // Load ALL entries 
    );
    
    std::cout << 
        (rawTransactions.wasInitialized()? 
        "> rawTransactions sucessfully initialized":
        "> Error when initializing rawTransactions!") 
    << std::endl;

    rawTransactions.print();
    //rawTransactions.printRowBinary(0);
}

// Convert Customer id from string to integer.
// Returns -1 if input string is an invalid ID.
int convertCustomerID(std::string customerID) {
    //if(!customerID.) return -1;
    if(customerID.empty()) return -1;

    std::stringstream buffer;
    bool prefixExtracted = false;
    bool intIDextracted = false;

    int intID=-1;

    for(int i=0; i<customerID.size(); i++) {
        char ichar = customerID[i];
        // Extract prefix first
        if(!prefixExtracted) { 
            if(' ' != ichar)
                buffer << ichar;
            prefixExtracted = (buffer.str() == std::string(CUSTID_PREFIX));
            if(prefixExtracted) {
                // Reset stringstream, in the next iteration the 
                // loop will start extracting the integer ID
                buffer = std::stringstream("");
            }
        } 
        // Extract integer ID
        else { 
            if(' ' != ichar) buffer << ichar;
        }
    }

    intIDextracted = prefixExtracted && !(buffer.str().empty());

    if(intIDextracted) {
       intID = atoi(buffer.str().c_str()); 
    } 

    return intID;
}

// STEP 2: Sort customer entries by date
void sortTransactionsByDate() {
    
    // First, extract the 'Date' column into an Array
    int nRows = rawTransactions.getnRows();

    DynamicArray_Sortable<int> transacIdArr = DynamicArray_Sortable<int>();
    DynamicArray_Sortable<int> customerIdArr = DynamicArray_Sortable<int>();
    DynamicArray_Sortable<Date> dateArr = DynamicArray_Sortable<Date>();
    
    //dateArr.setVerbose(true);
    //transacIdArr.setVerbose(true);

    // Generate the transacId column
    for(int i=0; i<nRows; i++) transacIdArr.pushBack(i);

    // Extract the 'Customer ID' column from the DataTable
    for(int i=0; i<nRows; i++) {
        int convertedId = convertCustomerID(
            rawTransactions.getStringAt(CUSTID_COLUMN_INDEX, i)
        ); 
        //std::cout << "> ID [" << i << "] = " << convertedId << std::endl;
        customerIdArr.pushBack(convertedId);
    }

    // Extract the 'Date' column from the DataTable
    for(int i=0; i<nRows; i++) {
        Date iDate = stdStringToDate(
            rawTransactions.getStringAt(DATE_COLUMN_INDEX, i)
        );
        //std::cout << "> Date [" << i << "] = " << iDate << std::endl;
        dateArr.pushBack(iDate);
    }

    //DynamicArray &parallelArrays[] = {transacIdArr, customerIdArr};

    //std::cout << "> Date array before quicksort:\n" << dateArr.getAsString() << std::endl;
    //std::cout << "> Transaction ID array before quicksort:\n" << transacIdArr.getAsString() << std::endl;

    std::cout << "============ SORTING ENTRIES, PLEASE WAIT ============" << std::endl;

    // Direct sorting of dates, parallel/mirrored sorting of idArr
    dateArr.quicksort<int>(0, dateArr.getSize()-1, transacIdArr);//customerIdArr);

    //std::cout << "> Date array after quicksort:\n" << dateArr.getAsString() << std::endl;
    //std::cout << "> Transaction ID array after quicksort:\n" << transacIdArr.getAsString() << std::endl;
    
    // Create the new table, unitialized
    sortedTransactions = DataTable();
    
    // Initialize the table correctly through 'reallocation'
    // (just allocation for this case)
    bool sortedTableAllocated = sortedTransactions.reallocate(
        rawTransactions.getnRows(), rawTransactions.getnCols(), 
        rawTransactions.getColumnTypes(), rawTransactions.getStringLengths());

    sortedTransactions.setColumnName(0, "Customer ID");
    sortedTransactions.setColumnName(1, "Product");
    sortedTransactions.setColumnName(2, "Category");
    sortedTransactions.setColumnName(3, "Price");
    sortedTransactions.setColumnName(4, "Date");
    sortedTransactions.setColumnName(5, "Payment Method");

    if(sortedTableAllocated)
        std::cout << "> Sorted Table allocated" <<std::endl;
    else {
        std::cout << "> Sorted Table failed to allocate..." << std::endl;
        return;
    }

    // Retrieve the entries in the order given by the indirectly sorted ID array
    for(int i=0; i<nRows; i++) {

        // Set Customer ID
        sortedTransactions.setStringAt(0, i, 
            rawTransactions.getStringAt(0, transacIdArr.getAt(i)));

        // Set Product
        sortedTransactions.setStringAt(1, i, 
            rawTransactions.getStringAt(1, transacIdArr.getAt(i)));
        
        // Set Category
        sortedTransactions.setStringAt(2, i, 
            rawTransactions.getStringAt(2, transacIdArr.getAt(i)));
                
        // Set Price
        sortedTransactions.setDoubleAt(3, i, 
            rawTransactions.getDoubleAt(3, transacIdArr.getAt(i)));
        
        // Set Date
        sortedTransactions.setStringAt(4, i,
            rawTransactions.getStringAt(4, transacIdArr.getAt(i)));
        
        // Set Payment Method
        sortedTransactions.setStringAt(5, i, 
            rawTransactions.getStringAt(5, transacIdArr.getAt(i)));
    }
    std::cout << "> Sorted Table loaded" << std::endl;
    // Then, display the table sorted by date:
    sortedTransactions.print();
}

// STEP 3: Display total number of transactions
void displayNtransactions() {
    std::cout << "> Total number of transactions = " << 
    sortedTransactions.getnRows() << std::endl;
}

int main(int argc, char** argv) { 
    
    loadTransactions();

    sortTransactionsByDate();
    
    displayNtransactions();    

    rawTransactions.free();
    sortedTransactions.free();

    return 0;
}

