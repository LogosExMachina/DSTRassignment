#include <iostream>
#include "shared/dataTable.h"
#include "shared/csvParser.h"
#include "dynamicArray/dynamicArray_sortable.hpp"
#include "shared/date.hpp"

// Solution 1: How can you efficiently sort customer transactions by date
// and display the total number of transactions in both datasets?

CSVParser parser;
DataTable rawTransactions, sortedTransactions;
int nTransactions = 0;
const int DATE_COLUMN_INDEX = 4;

// SETUP 1: Load the CSV data into a DataTable
void loadTransactions() {

    parser.setVerbose(false);

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
        columnTypes,
        stringLengths,
        nCols,
        //10 // test
        -1 // Load ALL entries
    );

    std::cout <<
        (rawTransactions.wasInitialized()?
        "> rawTransactions sucessfully initialized":
        "> Error when initializing rawTransactions!")
    << std::endl;

    //rawTransactions.print();
    //rawTransactions.printRowBinary(0);
}

// STEP 2: Sort customer entries by date
void sortTransactionsByDate() {

    // First, extract the 'Date' column into an Array
    int nRows = rawTransactions.getnRows();

    DynamicArray_Sortable<int> transacIdArr = DynamicArray_Sortable<int>();
    DynamicArray_Sortable<Date> dateArr = DynamicArray_Sortable<Date>();

    // Generate the transacId column
    for(int i=0; i<nRows; i++) transacIdArr.pushBack(i);

    // Extract the 'Date' column from the DataTable
    for(int i=0; i<nRows; i++) {
        Date iDate = stdStringToDate(
            rawTransactions.getStringAt(DATE_COLUMN_INDEX, i)
        );
        dateArr.pushBack(iDate);
    }

    nTransactions = dateArr.getSize();

    std::cout << "============ SORTING ENTRIES, PLEASE WAIT ============" << std::endl;
    
    dateArr.insertionSort(transacIdArr);

    int stopCounter=0;
    int stopLimit = 100;

    std::cout << "| ID | DATE |" << std::endl;
    for(int i=0; i<transacIdArr.getSize(); i++) {
        if(stopCounter>=stopLimit) {
            std::cout << "< Input anything to continue>";
            std::getchar();
            fflush(stdin);
            fflush(stdout);
            stopCounter=0;
        }
        
        std::cout << "| " << transacIdArr.getAt(i) << " | "
        << dateArr.getAt(i) << " |" << std::endl;
        stopCounter++;
    }
}

// STEP 3: Display total number of transactions
void displayNtransactions() {
    std::cout << "> Total number of transactions = " << 
    nTransactions
    << std::endl;
}

int main(int argc, char** argv) {

    loadTransactions();
    sortTransactionsByDate();
    displayNtransactions();

    rawTransactions.free();
    sortedTransactions.free();

    return 0;
}

