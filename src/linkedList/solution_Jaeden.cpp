#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include <algorithm>
#include <cctype>

#include "linkedList.hpp"

using namespace std;

// Task 1: Bubble sort for linked list of transactions by date (yyyy-mm-dd format)
void bubbleSortByDate(LinkedList<Transaction>& list) {
    if (list.getSize() < 2) return;
    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < list.getSize() - 1; i++) {
            Transaction& t1 = list.get(i);
            Transaction& t2 = list.get(i + 1);
            if (t2.getDateAsTm() < t1.getDateAsTm()) {
                std::swap(t1, t2);
                swapped = true;
            }
        }
    } while (swapped);
}

// Task 2: Percentage of Electronics purchases by Credit Card
void electronicsCreditCardStats(LinkedList<Transaction>& list) {
    int totalElectronics = 0, creditCardCount = 0;
    for (int i = 0; i < list.getSize(); i++) {
        Transaction& t = list.get(i);
        if (t.category == "Electronics") {
            totalElectronics++;
            if (t.paymentMethod == "Credit Card") {
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

// Task 3: Word Frequency in 1-star reviews
void analyze1StarReviews(LinkedList<Review>& list) {
    map<string, int> wordFreq;
    for (int i = 0; i < list.getSize(); i++) {
        Review& r = list.get(i);
        if (r.rating == 1) {
            stringstream ss(r.reviewText);
            string word;
            while (ss >> word) {
                word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                if (word.length() > 2) wordFreq[word]++;
            }
        }
    }
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

// Load transactions.csv into LinkedList<Transaction>
void loadTransactions(const string& filePath, LinkedList<Transaction>& list) {
    ifstream file(filePath);
    string line;
    getline(file, line); // header
    while (getline(file, line)) {
        stringstream ss(line);
        Transaction t;
        getline(ss, t.customerID, ',');
        getline(ss, t.product, ',');
        string priceStr;
        getline(ss, priceStr, ',');
        t.price = stod(priceStr);
        getline(ss, t.date, ',');
        getline(ss, t.category, ',');
        getline(ss, t.paymentMethod, ',');
        list.append(t);
    }
}

// Load reviews.csv into LinkedList<Review>
void loadReviews(const string& filePath, LinkedList<Review>& list) {
    ifstream file(filePath);
    string line;
    getline(file, line); // header
    while (getline(file, line)) {
        stringstream ss(line);
        Review r;
        string ratingStr;
        getline(ss, r.productID, ',');
        getline(ss, r.customerID, ',');
        getline(ss, ratingStr, ',');
        r.rating = stoi(ratingStr);
        getline(ss, r.reviewText);
        list.append(r);
    }
}

int main() {
    LinkedList<Transaction> transactions;
    LinkedList<Review> reviews;

    loadTransactions("transactions.csv", transactions);
    cout << "\nTotal Transactions (LinkedList): " << transactions.getSize() << endl;

    bubbleSortByDate(transactions);
    electronicsCreditCardStats(transactions);

    loadReviews("reviews.csv", reviews);
    analyze1StarReviews(reviews);

    return 0;
}
