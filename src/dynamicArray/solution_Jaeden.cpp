#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cctype>
#include <map>

#include "shared/csvParser.h"
#include "shared/dataTable.h"
#include "shared/date.hpp"
#include "dynamicArray/dynamicArray_sortable.hpp"
#include "shared/strUtils.h"

using namespace std;

CSVParser parser;
DataTable transTable, reviewTable;

// Convert date string to comparable Date
Date parseDate(const std::string& dateStr) {
    return stdStringToDate(dateStr);
}

// Task 1: Bubble Sort Transactions by Date
void bubbleSortByDate(DataTable& table, int dateCol) {
    int n = table.getnRows();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Date date1 = stdStringToDate(table.getStringAt(dateCol, j));
            Date date2 = stdStringToDate(table.getStringAt(dateCol, j + 1));
            if (date2 < date1) {
                // Swap all column values of row j and j+1
                for (int col = 0; col < table.getnCols(); col++) {
                    string temp = table.getStringAt(col, j);
                    table.setStringAt(col, j, table.getStringAt(col, j + 1));
                    table.setStringAt(col, j + 1, temp);
                }
            }
        }
    }
}

// Task 2: Percentage of Electronics purchases by Credit Card
void electronicsCreditCardStats(DataTable& table, int categoryCol, int paymentCol) {
    int totalElectronics = 0, creditCardCount = 0;
    for (int i = 0; i < table.getnRows(); i++) {
        string cat = table.getStringAt(categoryCol, i);
        string pay = table.getStringAt(paymentCol, i);
        if (cat == "Electronics") {
            totalElectronics++;
            if (pay == "Credit Card") {
                creditCardCount++;
            }
        }
    }
    if (totalElectronics > 0) {
        double percentage = (creditCardCount * 100.0) / totalElectronics;
        cout << "\nPercentage of Electronics purchases made by Credit Card: " << percentage << "%\n";
    } else {
        cout << "\nNo Electronics purchases found.\n";
    }
}

// Task 3: Most Frequent Words in 1-Star Reviews
void analyze1StarReviews(DataTable& reviews, int ratingCol, int textCol) {
    map<string, int> wordFreq;
    for (int i = 0; i < reviews.getnRows(); i++) {
        int rating = reviews.getIntAt(ratingCol, i);
        if (rating == 1) {
            stringstream ss(reviews.getStringAt(textCol, i));
            string word;
            while (ss >> word) {
                // Clean word (lowercase, strip punctuation)
                word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                if (word.length() > 2) wordFreq[word]++;
            }
        }
    }
    // Print top 10 words
    cout << "\nTop Words in 1-Star Reviews:\n";
    for (int i = 0; i < 10 && !wordFreq.empty(); i++) {
        auto maxIt = max_element(wordFreq.begin(), wordFreq.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) {
                return a.second < b.second;
            });
        cout << maxIt->first << ": " << maxIt->second << " times\n";
        wordFreq.erase(maxIt);
    }
}

int main() {

    parser.setVerbose(false);

    // Parse transactions
    ColumnType transTypes[] = 
        {ColumnType::STRING, ColumnType::STRING, ColumnType::STRING,
        ColumnType::DOUBLE, ColumnType::STRING, ColumnType::STRING};

    int transStrLens[] = {16, 32, 16, -1, 16, 32};
    int nCols = sizeof(transTypes)/sizeof(ColumnType);

    transTable = parser.parseCSV("data\\transactions_cleaned.csv", transTypes, transStrLens, nCols, 10);
    std::cout <<
    (transTable.wasInitialized()?
    "> rawTransactions sucessfully initialized":
    "> Error when initializing rawTransactions!")
    << std::endl;

    // Task 1: Sort by date

    int nRows = transTable.getnRows();

    // Extract the transaction IDs and the dates as arrays
    DynamicArray_Sortable<int> transacIdArr = DynamicArray_Sortable<int>();
    DynamicArray_Sortable<Date> dateArr = DynamicArray_Sortable<Date>();

    // Generate the transacId column
    for(int i=0; i<nRows; i++) transacIdArr.pushBack(i);
    
    // Extract the 'Date' column from the DataTable
    int dateCol = 4; // index of date
    for(int i=0; i<nRows; i++) {
        Date iDate = stdStringToDate(
            transTable.getStringAt(dateCol, i)
        );
        //std::cout << "> Date [" << i << "] = " << iDate << std::endl;
        dateArr.pushBack(iDate);
    }
    
    dateArr.bubblesort();

    std::cout << "Transactions sorted by ID:" << std::endl;
    std::cout << "| ID |  DATE  |" << std::endl;
    for(int i=0; i<nRows; i++) {
        std::cout << "| " << transacIdArr.getAt(i) << " | "
        << dateArr.getAt(i).getAsString() << " |" << std::endl;
    }

    cout << "\nTotal Transactions (Array): " << transTable.getnRows() << endl;

    std::cout << "< Input anything to continue >" << std::endl;
    std::getchar();
    
    
    // Task 2: Percentage of Electronics purchases by Credit Card
    int catCol = 2, payCol = 5;
    electronicsCreditCardStats(transTable, catCol, payCol);
    

    // Parse reviews
    
    ColumnType revTypes[] = {ColumnType::STRING, ColumnType::STRING, ColumnType::INT, ColumnType::STRING};
    int revStrLens[] = {20, 20, -1, 200};
    reviewTable = parser.parseCSV("data/reviews_cleaned.csv", revTypes, revStrLens, 4, 10);
    

    // Task 3
    std::cout << "< Input anything to continue >" << std::endl;
    std::getchar();
    
    analyze1StarReviews(reviewTable, 2, 3);
    
    
    transTable.free();
    reviewTable.free();

    return 0;
}