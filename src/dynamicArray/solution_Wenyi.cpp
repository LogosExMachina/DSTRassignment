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

    // convert to array
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

    // convert to array
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
template <typename T>
DynamicArray_Unsortable<T> merge(DynamicArray_Unsortable<T>& left, DynamicArray_Unsortable<T>& right, bool (*cmp)(const T&, const T&)) {
    if (left.getSize() == 0) return right;
    if (right.getSize() == 0) return left;

    DynamicArray_Unsortable<T> result(left.getSize() + right.getSize());
    result.setVerbose(false);

    int i = 0, j = 0;
    while (i < left.getSize() && j < right.getSize()) {
        if (cmp(left.getAt(i), right.getAt(j))) {
            result.pushBack(left.getAt(i));
            i++;
        } else {
            result.pushBack(right.getAt(j));
            j++;
        }
    }
    while (i < left.getSize()) result.pushBack(left.getAt(i++));
    while (j < right.getSize()) result.pushBack(right.getAt(j++));

    return result;
}

template <typename T>
DynamicArray_Unsortable<T> mergeSort(DynamicArray_Unsortable<T>& data, bool (*cmp)(const T&, const T&)) {
    int size = data.getSize();
    if (size < 2) return data;

    int mid = size / 2;
    DynamicArray_Unsortable<T> left(mid);
    DynamicArray_Unsortable<T> right(size - mid);
    left.setVerbose(false);
    right.setVerbose(false);

    for (int i = 0; i < mid; i++) left.pushBack(data.getAt(i));
    for (int i = mid; i < size; i++) right.pushBack(data.getAt(i));

    auto sortedLeft = mergeSort(left, cmp);
    auto sortedRight = mergeSort(right, cmp);

    return merge(sortedLeft, sortedRight, cmp);
}


// linear search
template <typename T>
DynamicArray_Unsortable<T> filter(DynamicArray_Unsortable<T>& data, bool (*cmp)(const T&, const T&)) {
    DynamicArray_Unsortable<T> filtered;
    filtered.setVerbose(false);

    for (int i = 0; i < data.getSize(); i++) {
        if (cmp(data.getAt(i), T())) {
            filtered.pushBack(data.getAt(i));
        }
    }

    return filtered;
}

// word
DynamicArray_Unsortable<WordFrequency> analyzeReviewWords(DynamicArray_Unsortable<Review>& reviews) {
    DynamicArray_Unsortable<WordFrequency> wordFrequencies;
    wordFrequencies.setVerbose(false);

    int size = reviews.getSize();
    for (int i = 0; i < size; i++) {
        string reviewText = reviews.getAt(i).reviewText;
        istringstream iss(reviewText);
        string word;

        // for each word
        while (iss >> word) {

            // check if the word is already in the list
            bool found = false;
            for (int j = 0; j < wordFrequencies.getSize(); j++) {
                if (wordFrequencies.getAt(j).word == word) {
                    int frequency = wordFrequencies.getAt(j).frequency;
                    WordFrequency wf(word, ++frequency);
                    wordFrequencies.insertAt(wf, j);
                    found = true;
                    break;
                }
            }

            // if not, add it to the list
            if (!found) {
                WordFrequency wf(word, 1);
                wordFrequencies.pushBack(wf);
            }
        }
    }

    // sort
    auto sorted = mergeSort<WordFrequency>(wordFrequencies, [](const WordFrequency& a, const WordFrequency& b) {
        return a > b;
    });

    return sorted;
}


// main
int main (int argc, char *argv[]) {
    // load csv
    DynamicArray_Unsortable<Transaction> transactions = loadTransactions("data/transactions_cleaned.csv");
    DynamicArray_Unsortable<Review> reviews = loadReviews("data/reviews_cleaned.csv");
    if(!transactions.wasInitialized()) return 1;
    if(!reviews.wasInitialized()) return 1;

    cout << "Loaded " << transactions.getSize() << " transactions and " << reviews.getSize() << " reviews." << endl;

    // question 1: sort transactions by date
    DynamicArray_Unsortable<Transaction> sortedTransactions = mergeSort<Transaction>(transactions, [](const Transaction& a, const Transaction& b) {
        return a.date < b.date;
    });

    cout << sortedTransactions.getSize() << " transactions sorted by date." << endl;
    int lim = min(10, sortedTransactions.getSize());
    for (int i = 0; i < lim; i++) {
        cout << sortedTransactions.getAt(i) << endl;
    }

    // question 2: what percentage of purchases in the “electronics” category were made using credit card payments?
    DynamicArray_Unsortable<Transaction> electronicsTransactions = filter<Transaction>(transactions, [](const Transaction& t, const Transaction&) {
        return t.category == "Electronics";
    });
    DynamicArray_Unsortable<Transaction> creditCardTransactions = filter<Transaction>(electronicsTransactions, [](const Transaction& t, const Transaction&) {
        return t.paymentMethod == "Credit Card";
    });

    double percentage = electronicsTransactions.getSize() == 0 ? 0 :
        (double)creditCardTransactions.getSize() / electronicsTransactions.getSize() * 100;
    cout << "Percentage of electronics purchases made using credit card payments: " << percentage << "%" << endl;

    // question 3: which words are most frequently used in product reviews rated 1-star?
    DynamicArray_Unsortable<Review> oneStarReviews = filter<Review>(reviews, [](const Review& r, const Review&) {
        return r.rating == 1;
    });
    DynamicArray_Unsortable<WordFrequency> wordFrequencies = analyzeReviewWords(oneStarReviews);
    cout << "Most frequently used words in 1-star reviews:" << endl;
    for (int i = 0; i < wordFrequencies.getSize(); i++) {
        cout << wordFrequencies.getAt(i).word << ": " << wordFrequencies.getAt(i).frequency << endl;
    }

    return 0;
}
