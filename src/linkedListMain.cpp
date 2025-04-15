#include "linkedList.hpp"
#include <vector>
#include <map>
#include <chrono>
#include <algorithm>

// Function to load transactions from CSV file
LinkedList<Transaction> loadTransactions(const std::string& filename) {
    LinkedList<Transaction> transactions;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return transactions;
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        Transaction transaction;

        // Parse CSV columns
        std::string priceStr;
        std::getline(ss, transaction.customerID, ',');
        std::getline(ss, transaction.product, ',');
        std::getline(ss, transaction.category, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, transaction.date, ',');
        std::getline(ss, transaction.paymentMethod, ',');

        // Convert price string to double
        try {
            transaction.price = std::stod(priceStr);
            transactions.append(transaction);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing price: " << priceStr << std::endl;
        }
    }

    file.close();
    return transactions;
}

// Function to load reviews from CSV file
LinkedList<Review> loadReviews(const std::string& filename) {
    LinkedList<Review> reviews;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return reviews;
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        Review review;

        // Parse CSV columns
        std::string ratingStr;
        std::getline(ss, review.productID, ',');
        std::getline(ss, review.customerID, ',');
        std::getline(ss, ratingStr, ',');
        std::getline(ss, review.reviewText);

        // Convert rating string to int
        try {
            review.rating = std::stoi(ratingStr);
            reviews.append(review);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing rating: " << ratingStr << std::endl;
        }
    }

    file.close();
    return reviews;
}

// Function to analyze word frequency in reviews
std::map<std::string, int> analyzeReviewWords(const LinkedList<Review>& reviews) {
    std::map<std::string, int> wordFrequency;

    LinkedList<Review>::Iterator it = reviews.iterator();
    while (it.hasNext()) {
        Review review = it.next();
        std::istringstream ss(review.reviewText);
        std::string word;

        while (ss >> word) {
            // Remove punctuation
            word.erase(std::remove_if(word.begin(), word.end(),
                     [](char c) { return std::ispunct(static_cast<unsigned char>(c)); }),
                     word.end());

            // Convert to lowercase
            std::transform(word.begin(), word.end(), word.begin(),
                     [](unsigned char c) { return std::tolower(c); });

            // Increment frequency
            if (!word.empty()) {
                wordFrequency[word]++;
            }
        }
    }

    return wordFrequency;
}

// Utility function to print most frequent words
void printMostFrequentWords(const std::map<std::string, int>& wordFrequency, int topN) {
    // Create vector of pairs from map
    std::vector<std::pair<std::string, int>> wordPairs(wordFrequency.begin(), wordFrequency.end());

    // Sort by frequency in descending order
    std::sort(wordPairs.begin(), wordPairs.end(),
             [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                 return a.second > b.second;
             });

    std::cout << "Most frequent words in reviews:\n";
    int count = 0;
    for (const auto& pair : wordPairs) {
        if (count >= topN) break;
        std::cout << pair.first << ": " << pair.second << " occurrences\n";
        count++;
    }
}

int main() {
    std::cout << "E-Commerce Data Analysis using Linked Lists\n";
    std::cout << "===========================================\n\n";

    // Load transaction data
    auto startTime = std::chrono::high_resolution_clock::now();
    LinkedList<Transaction> transactions = loadTransactions("data/transactions_cleaned.csv");
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Loaded " << transactions.getSize() << " transactions in " << duration << " ms\n";

    // Load review data
    startTime = std::chrono::high_resolution_clock::now();
    LinkedList<Review> reviews = loadReviews("data/reviews_cleaned.csv");
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Loaded " << reviews.getSize() << " reviews in " << duration << " ms\n\n";

    // Question 1: Sort transactions by date
    std::cout << "Question 1: Sorting transactions by date...\n";
    startTime = std::chrono::high_resolution_clock::now();
    transactions.mergeSort();
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Sorted " << transactions.getSize() << " transactions in " << duration << " ms\n";
    std::cout << "Total number of transactions: " << transactions.getSize() << "\n";
    std::cout << "Total number of reviews: " << reviews.getSize() << "\n\n";

    // Print first 5 transactions to verify sorting
    std::cout << "First 5 transactions after sorting:\n";
    for (int i = 0; i < 5 && i < transactions.getSize(); i++) {
        transactions.get(i).print();
    }
    std::cout << "\n";

    // Question 2: Calculate percentage of Electronics category purchases using Credit Card
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

    // Question 3: Find most frequent words in 1-star reviews
    std::cout << "Question 3: Analyzing words in 1-star reviews...\n";
    startTime = std::chrono::high_resolution_clock::now();

    LinkedList<Review> oneStarReviews = reviews.searchByRating(1);
    std::map<std::string, int> wordFrequency = analyzeReviewWords(oneStarReviews);

    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Found " << oneStarReviews.getSize() << " 1-star reviews\n";
    std::cout << "Word analysis completed in " << duration << " ms\n\n";

    // Print top 10 most frequent words in 1-star reviews
    printMostFrequentWords(wordFrequency, 10);

    return 0;
}
