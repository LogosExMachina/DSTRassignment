#include "csvParser.h"
#include "dataTable.h"
#include "linkedList.hpp"

#include <chrono>
#include <algorithm>
#include <chrono>

// parse csv
LinkedList<Transaction> loadTransactions(const std::string& filename) {
    LinkedList<Transaction> transactions;

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
        std::cout << "Error when loading table from CSV" << std::endl;
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

        transactions.append(transaction);
    }

    return transactions;
}

LinkedList<Review> loadReviews(const std::string& filename) {
    LinkedList<Review> reviews;

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
        std::cout << "Error when loading table from CSV" << std::endl;
        return reviews;
    }

    // convert to linked list
    for (int i = 0; i < table.getnRows(); i++) {
        Review review;
        review.productID = table.getStringAt(0, i);
        review.customerID = table.getStringAt(1, i);
        review.rating = table.getIntAt(2, i);
        review.reviewText = table.getStringAt(3, i);

        reviews.append(review);
    }

    return reviews;
}

// analyze word frequency in reviews
LinkedList<WordFrequency> analyzeReviewWords(const LinkedList<Review>& reviews) {
    LinkedList<WordFrequency> wordFrequency;

    LinkedList<Review>::Iterator it = reviews.iterator();
    while (it.hasNext()) {
        Review review = it.next();
        std::istringstream ss(review.reviewText);
        std::string word;

        while (ss >> word) {
            // remove punctuation
            word.erase(std::remove_if(word.begin(), word.end(),
                     [](char c) { return std::ispunct(static_cast<unsigned char>(c)); }),
                     word.end());

            // convert to lowercase
            std::transform(word.begin(), word.end(), word.begin(),
                     [](unsigned char c) { return std::tolower(c); });

            // increment frequency
            if (!word.empty()) {
                WordFrequency* wf = wordFrequency.getWord(word);
                // if word already exists, increment frequency
                if (wf != nullptr) {
                    wf->frequency++;
                } else {
                    wordFrequency.append(WordFrequency(word, 1));
                }
            }
        }
    }

    // sort
    wordFrequency.mergeSort();

    return wordFrequency;
}

// utility function to print most frequent words
void printMostFrequentWords(const LinkedList<WordFrequency>& wordFrequency, int topN) {
    std::cout << "Most frequent words in 1-star reviews:\n";
    std::cout << "---------------------------------------\n";

    int size = wordFrequency.getSize();
    int startIndex = size - topN;
    if (startIndex < 0) startIndex = 0;

    LinkedList<WordFrequency>::Iterator it = wordFrequency.iterator();
    int count = 0;

    while (it.hasNext()) {
        WordFrequency wf = it.next();
        if (count >= startIndex) {
            std::cout << wf.word << ": " << wf.frequency << "\n";
        }
        count++;
    }

    std::cout << "\n";
}

int main() {
    std::cout << "E-Commerce Data Analysis using Linked Lists\n";
    std::cout << "===========================================\n\n";

    // load transaction data
    auto startTime = std::chrono::high_resolution_clock::now();
    LinkedList<Transaction> transactions = loadTransactions("data/transactions_cleaned.csv");
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Loaded " << transactions.getSize() << " transactions in " << duration << " ms\n";

    // load review data
    startTime = std::chrono::high_resolution_clock::now();
    LinkedList<Review> reviews = loadReviews("data/reviews_cleaned.csv");
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Loaded " << reviews.getSize() << " reviews in " << duration << " ms\n\n";

    // question 1: sort transactions by date
    std::cout << "Question 1: Sorting transactions by date...\n";
    startTime = std::chrono::high_resolution_clock::now();
    transactions.mergeSort();
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Sorted " << transactions.getSize() << " transactions in " << duration << " ms\n";
    std::cout << "Total number of transactions: " << transactions.getSize() << "\n";
    std::cout << "Total number of reviews: " << reviews.getSize() << "\n\n";

    // print first 5 transactions to verify sorting
    std::cout << "First 5 transactions after sorting:\n";
    for (int i = 0; i < 5 && i < transactions.getSize(); i++) {
        transactions.get(i).print();
    }
    std::cout << "\n";

    // question 2: calculate percentage of electronics category purchases using credit card
    std::cout << "Question 2: Analyzing Electronics category purchases...\n";
    startTime = std::chrono::high_resolution_clock::now();

    LinkedList<Transaction> electronicsTransactions = transactions.searchByCategory("Electronics");
    LinkedList<Transaction> electronicsCreditCardTransactions =
        transactions.searchByCategoryAndPayment("Electronics", "Credit Card");

    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    int totalElectronics = electronicsTransactions.getSize();
    int electronicsCreditCard = electronicsCreditCardTransactions.getSize();

    double percentage = (totalElectronics > 0) ?
        (static_cast<double>(electronicsCreditCard) / totalElectronics) * 100 : 0;

    std::cout << "Total Electronics transactions: " << totalElectronics << "\n";
    std::cout << "Electronics with Credit Card payments: " << electronicsCreditCard << "\n";
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) << percentage << "%\n";
    std::cout << "Search and calculation completed in " << duration << " ms\n\n";

    // question 3: find most frequent words in 1-star reviews
    std::cout << "Question 3: Analyzing words in 1-star reviews...\n";
    startTime = std::chrono::high_resolution_clock::now();

    LinkedList<Review> oneStarReviews = reviews.searchByRating(1);
    LinkedList<WordFrequency> wordFrequency = analyzeReviewWords(oneStarReviews);

    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Found " << oneStarReviews.getSize() << " 1-star reviews\n";
    std::cout << "Word analysis completed in " << duration << " ms\n\n";

    printMostFrequentWords(wordFrequency, 10);

    return 0;
}
