
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

// Custom dynamic array implementation
template <typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}
    
    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size >= capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T* newData = new T[newCapacity];
            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
        data[size++] = value;
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    size_t getSize() const { return size; }
};

// Transaction structure
struct Transaction {
    char customerID[20];
    char product[50];
    char category[50];
    double price;
    int date; // YYYYMMDD format
    char paymentMethod[50];
};

// Function to convert date from DD/MM/YYYY to YYYYMMDD
int convertDate(const char* dateStr) {
    if (strlen(dateStr) != 10) return 0; // Invalid date
    
    int day = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');
    int month = (dateStr[3] - '0') * 10 + (dateStr[4] - '0');
    int year = (dateStr[6] - '0') * 1000 + (dateStr[7] - '0') * 100 + 
               (dateStr[8] - '0') * 10 + (dateStr[9] - '0');
    
    if (month < 1 || month > 12 || day < 1 || day > 31) return 0;
    return year * 10000 + month * 100 + day;
}

// Bucket sort implementation
void bucketSort(DynamicArray<Transaction>& transactions) {
    if (transactions.getSize() <= 1) return;

    // Find min and max dates
    int minDate = 99999999;
    int maxDate = 0;
    for (size_t i = 0; i < transactions.getSize(); ++i) {
        if (transactions[i].date < minDate && transactions[i].date > 0) {
            minDate = transactions[i].date;
        }
        if (transactions[i].date > maxDate) {
            maxDate = transactions[i].date;
        }
    }

    // Create buckets (one per month)
    const int NUM_BUCKETS = 12;
    DynamicArray<Transaction> buckets[NUM_BUCKETS];

    // Distribute transactions into buckets
    for (size_t i = 0; i < transactions.getSize(); ++i) {
        if (transactions[i].date > 0) {
            int month = (transactions[i].date / 100) % 100 - 1;
            buckets[month].push_back(transactions[i]);
        }
    }

    // Sort each bucket and collect results
    size_t index = 0;
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        // Simple insertion sort for each bucket
        for (size_t j = 1; j < buckets[i].getSize(); ++j) {
            Transaction key = buckets[i][j];
            int k = j - 1;
            while (k >= 0 && buckets[i][k].date > key.date) {
                buckets[i][k + 1] = buckets[i][k];
                k--;
            }
            buckets[i][k + 1] = key;
        }

        // Collect sorted transactions
        for (size_t j = 0; j < buckets[i].getSize(); ++j) {
            transactions[index++] = buckets[i][j];
        }
    }
}

int main() {
    DynamicArray<Transaction> transactions;
    int transactionCount = 0;
    int reviewCount = 0;

    // Read transactions.csv
    std::ifstream transFile("data/transactions.csv");
    if (!transFile) {
        std::cerr << "Error: Cannot open transactions.csv\n";
        return 1;
    }

    char line[256];
    // Skip header
    transFile.getline(line, sizeof(line));

    while (transFile.getline(line, sizeof(line))) {
        Transaction trans;
        char dateStr[11];
        char* token = strtok(line, ",");
        
        if (!token) continue;
        strncpy(trans.customerID, token, sizeof(trans.customerID) - 1);
        trans.customerID[sizeof(trans.customerID) - 1] = '\0';

        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(trans.product, token, sizeof(trans.product) - 1);
        trans.product[sizeof(trans.product) - 1] = '\0';

        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(trans.category, token, sizeof(trans.category) - 1);
        trans.category[sizeof(trans.category) - 1] = '\0';

        if (!(token = strtok(nullptr, ","))) continue;
        trans.price = atof(token);

        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(dateStr, token, sizeof(dateStr) - 1);
        dateStr[sizeof(dateStr) - 1] = '\0';
        trans.date = convertDate(dateStr);

        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(trans.paymentMethod, token, sizeof(trans.paymentMethod) - 1);
        trans.paymentMethod[sizeof(trans.paymentMethod) - 1] = '\0';

        if (trans.date > 0) { // Only add valid transactions
            transactions.push_back(trans);
        }
        transactionCount++;
    }
    transFile.close();

    // Count reviews
    std::ifstream reviewFile("data/reviews.csv");
    if (!reviewFile) {
        std::cerr << "Error: Cannot open reviews.csv\n";
        return 1;
    }

    // Skip header
    reviewFile.getline(line, sizeof(line));
    
    while (reviewFile.getline(line, sizeof(line))) {
        reviewCount++;
    }
    reviewFile.close();

    // Sort transactions by date
    bucketSort(transactions);

    // Display results
    std::cout << "----------------------------------------" << std::endl;
    for (size_t i = 0; i < transactions.getSize(); ++i) {
        if (transactions[i].date > 0) {
            std::cout << "Date: " << transactions[i].date << " | "
            << "Customer: " << transactions[i].customerID << " | "
            << "Product: " << transactions[i].product << " | "
            << "Price: " << transactions[i].price << std::endl;
        }
    }
    std::cout << "Total number of transactions: " << transactionCount << std::endl;
    std::cout << "Total number of reviews: " << reviewCount << std::endl;
    std::cout << "\nSorted transactions by date:" << std::endl;

    return 0;
}