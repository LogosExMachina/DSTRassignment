#include <iostream>
#include "dataTable.h"
#include "csvParser.h"
#include "dynamicArray_sortable.hpp"
#include "date.hpp"
#include "strUtils.h"

// Solution 2: What percentage of purchases in the “Electronics”
// category were made using Credit Card payments?
// (Hint: You will need to search and filter transactions
// based on category and payment method.)

CSVParser parser;
DataTable rawTransactions;// filteredTransactions;
DynamicArray_Sortable<int> electronicsIDs;
int nElectronics=0, nCreditCardPurchases=0;
const int CATEGORY_COLUMN_INDEX = 2;
const int PAYMETHOD_COLUMN_INDEX = 5;

// STEP 1: Load transaction entries
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
}

// STEP 2: Filter categories by 'Electronics' and count
void filterElectronics() {

    electronicsIDs = DynamicArray_Sortable<int>();

    // Filter
    for(int i=0; i<rawTransactions.getnRows(); i++) {
        std::string icategory = rawTransactions.getStringAt(CATEGORY_COLUMN_INDEX, i);
        trimTrailingInPlace_STL(icategory);
        trimLeadingInPlace_STL(icategory);

        if(icategory == "Electronics") {
            electronicsIDs.pushBack(i);
        }
    }

    // Get count
    nElectronics = electronicsIDs.getSize();
}

// STEP 3: Count how many entries used 'Credit Card' as payment,
// then calculate it as a percentage from the 'Electronics' category
void calculatePercentage() {

    // Filter by 'Credit Card' Payment method
    for(int i=0; i<nElectronics; i++) {
        std::string ipayMethod = rawTransactions.getStringAt(PAYMETHOD_COLUMN_INDEX, i);
        trimTrailingInPlace_STL(ipayMethod);
        trimLeadingInPlace_STL(ipayMethod);

        if(ipayMethod == "Credit Card") {
            nCreditCardPurchases++;
        }
    }

    std::cout << "> Number of purchases in 'Electronics' = "
    << nElectronics << std::endl;
    std::cout << "> Number of purchases using 'Credit Card' = "
    << nCreditCardPurchases << std::endl;

    double creditCardPercent = ((double)nCreditCardPurchases / (double)nElectronics) * 100.0;
    std::cout << "> Percentage = " << creditCardPercent << "%" << std::endl;
}

int main(int argc, char** argv) {

    loadTransactions();
    filterElectronics();
    calculatePercentage();

    rawTransactions.free();
    //filteredTransactions.free();

    return 0;
}
