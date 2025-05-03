#include <algorithm>
#include <string>

#include "csvParser.h"
#include "dynamicArray_unsortable.hpp"
#include "linkedList.hpp"

using namespace std;


// load
DynamicArray_Unsortable<Transaction> loadTransactions(const string filename) {
    DynamicArray_Unsortable<Transaction> transactions;
    transactions.setVerbose(false);

    // santi's csv parser
    CSVParser parser;
    ColumnType colTypes[] = {
        ColumnType::STRING, // Customer ID
        ColumnType::STRING, // Product
        ColumnType::STRING, // Category
        ColumnType::DOUBLE, // Price
        ColumnType::STRING, // Date
        ColumnType::STRING  // Payment Method
    };
    int maxStringLengths[] = {
        16,
        16,
        16,
        -1,
        16,
        16,
    };
    int nCols = sizeof(colTypes)/sizeof(ColumnType);
    DataTable table = parser.parseCSV("data/transactions_cleaned.csv", colTypes, maxStringLengths, nCols, -1);

    if(!table.wasInitialized()) {
        cout << "Error when loading table from CSV" << endl;
        return transactions;
    }

    // convert to linked list
    for (int i = 0; i < table.getnRows(); i++) {
        Transaction transaction;
        transaction.customerID = table.getStringAt(0, i);
        transaction.product = table.getStringAt(1, i);
        transaction.category = table.getStringAt(2, i);
        transaction.price = table.getDoubleAt(3, i);
        transaction.date = table.getStringAt(4, i);
        transaction.paymentMethod = table.getStringAt(5, i);

        transactions.pushBack(transaction);
    }

    return transactions;
}

DynamicArray_Unsortable<Review> loadReviews(const string& filename) {
    DynamicArray_Unsortable<Review> reviews;

    // santi's csv parser
    CSVParser parser;
    ColumnType colTypes[] = {
        ColumnType::STRING, // Product ID
        ColumnType::STRING, // Customer ID
        ColumnType::INT,    // Rating
        ColumnType::STRING  // Review Text
    };
    int maxStringLengths[] = {
        16,
        16,
        -1,
        256,
    };
    int nCols = sizeof(colTypes)/sizeof(ColumnType);
    DataTable table = parser.parseCSV("data/reviews_cleaned.csv", colTypes, maxStringLengths, nCols, -1);
    if(!table.wasInitialized()) {
        cout << "Error when loading table from CSV" << endl;
        return reviews;
    }

    // convert to linked list
    for (int i = 0; i < table.getnRows(); i++) {
        Review review;
        review.productID = table.getStringAt(0, i);
        review.customerID = table.getStringAt(1, i);
        review.rating = table.getIntAt(2, i);
        review.reviewText = table.getStringAt(3, i);

        reviews.pushBack(review);
    }

    return reviews;
}


// merge sort
DynamicArray_Unsortable<Transaction> merge(DynamicArray_Unsortable<Transaction>& left, DynamicArray_Unsortable<Transaction>& right) {
    if (left.getSize() == 0) return right;
    if (right.getSize() == 0) return left;

    DynamicArray_Unsortable<Transaction> result(left.getSize() + right.getSize());
    result.setVerbose(false);

    int i = 0, j = 0, k = 0;
    while (i < left.getSize() && j < right.getSize()) {
        if (left.getAt(i).date <= right.getAt(j).date) {
            result.pushBack(left.getAt(i));
            i++;
        } else {
            result.pushBack(right.getAt(j));
            j++;
        }
    }
    while (i < left.getSize()) {
        result.pushBack(left.getAt(i));
        i++;
    }
    while (j < right.getSize()) {
        result.pushBack(right.getAt(j));
        j++;
    }

    return result;
}

DynamicArray_Unsortable<Transaction> mergeSort(DynamicArray_Unsortable<Transaction>& transactions) {
    int size = transactions.getSize();
    if (size < 2) return transactions;

    int mid = size / 2;

    DynamicArray_Unsortable<Transaction> left(mid);
    DynamicArray_Unsortable<Transaction> right(size - mid);
    left.setVerbose(false);
    right.setVerbose(false);

    for (int i = 0; i < mid; i++) {
        left.pushBack(transactions.getAt(i));
    }
    for (int i = mid; i < size; i++) {
        right.pushBack(transactions.getAt(i));
    }

    auto sortedLeft = mergeSort(left);
    auto sortedRight = mergeSort(right);

    return merge(sortedLeft, sortedRight);
}

int main (int argc, char *argv[]) {
    // load csv
    DynamicArray_Unsortable<Transaction> transactions = loadTransactions("data/transactions_cleaned.csv");
    DynamicArray_Unsortable<Review> reviews = loadReviews("data/reviews_cleaned.csv");
    if(!transactions.wasInitialized()) return 1;
    if(!reviews.wasInitialized()) return 1;

    cout << "Loaded " << transactions.getSize() << " transactions and " << reviews.getSize() << " reviews." << endl;

    // question 1: sort transactions by date
    DynamicArray_Unsortable<Transaction> sortedTransactions = mergeSort(transactions);
    cout << sortedTransactions.getSize() << " transactions sorted by date." << endl;
    int lim = sortedTransactions.getSize();
    for (int i = 0; i < lim; i++) {
        cout << sortedTransactions.getAt(i) << endl;
    }

    return 0;
}
