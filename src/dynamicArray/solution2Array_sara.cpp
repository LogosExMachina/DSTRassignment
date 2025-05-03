
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>

// Transaction structure
struct Transaction {
    char customerID[20];
    char product[50];
    char category[50];
    double price;
    int date; // YYYYMMDD format
    char paymentMethod[50];
};

// Node structure for singly linked list
struct Node {
    Transaction data;
    Node* next;
    Node(const Transaction& t) : data(t), next(nullptr) {}
};

// Custom Linked List class
class LinkedList {
private:
    Node* head;
    size_t size;

    // Merge sort helper functions
    Node* merge(Node* first, Node* second) {
        if (!first) return second;
        if (!second) return first;

        Node* result = nullptr;
        if (first->data.date <= second->data.date) {
            result = first;
            result->next = merge(first->next, second);
        } else {
            result = second;
            result->next = merge(first, second->next);
        }
        return result;
    }

    Node* getMiddle(Node* head) {
        if (!head) return nullptr;
        Node *slow = head, *fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    Node* mergeSortHelper(Node* head) {
        if (!head || !head->next) return head;

        Node* middle = getMiddle(head);
        Node* nextToMiddle = middle->next;
        middle->next = nullptr;

        Node* left = mergeSortHelper(head);
        Node* right = mergeSortHelper(nextToMiddle);

        return merge(left, right);
    }

public:
    LinkedList() : head(nullptr), size(0) {}
    
    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void insert(const Transaction& t) {
        Node* newNode = new Node(t);
        newNode->next = head;
        head = newNode;
        size++;
    }

    void mergeSort() {
        head = mergeSortHelper(head);
    }

    // Function to convert date from DD/MM/YYYY to YYYYMMDD
    static int convertDate(const char* dateStr) {
        if (strlen(dateStr) != 10) return 0;
        
        int day = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');
        int month = (dateStr[3] - '0') * 10 + (dateStr[4] - '0');
        int year = (dateStr[6] - '0') * 1000 + (dateStr[7] - '0') * 100 + 
                  (dateStr[8] - '0') * 10 + (dateStr[9] - '0');
        
        if (month < 1 || month > 12 || day < 1 || day > 31) return 0;
        return year * 10000 + month * 100 + day;
    }

    // Search and count Electronics purchases with Credit Card
    std::pair<int, double> searchElectronicsCreditCard() const {
        int totalTransactions = 0;
        int matchingTransactions = 0;
        Node* current = head;
        
        while (current) {
            totalTransactions++;
            if (strcmp(current->data.category, "Electronics") == 0 && 
                strcmp(current->data.paymentMethod, "Credit Card") == 0) {
                matchingTransactions++;
            }
            current = current->next;
        }
        
        double percentage = totalTransactions > 0 ? 
            (static_cast<double>(matchingTransactions) / totalTransactions) * 100 : 0;
        return std::make_pair(matchingTransactions, percentage);
    }

    void display() const {
        Node* current = head;
        while (current) {
            std::cout << "Date: " << current->data.date << " | "
                     << "Customer: " << current->data.customerID << " | "
                     << "Category: " << current->data.category << " | "
                     << "Payment: " << current->data.paymentMethod << " | "
                     << "Price: $" << std::fixed << std::setprecision(2) 
                     << current->data.price << std::endl;
            current = current->next;
        }
    }

    size_t getSize() const { return size; }
};

int main() {
    LinkedList transactions;

    // Read transactions.csv
    std::ifstream file("data/transactions.csv");
    if (!file) {
        std::cerr << "Error: Cannot open transactions.csv\n";
        return 1;
    }

    char line[256];
    // Skip header
    file.getline(line, sizeof(line));

    // Read and parse transactions
    while (file.getline(line, sizeof(line))) {
        Transaction trans;
        char dateStr[11];
        char* token = strtok(line, ",");
        
        // Parse CustomerID
        if (!token) continue;
        strncpy(trans.customerID, token, sizeof(trans.customerID) - 1);
        trans.customerID[sizeof(trans.customerID) - 1] = '\0';

        // Parse Product
        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(trans.product, token, sizeof(trans.product) - 1);
        trans.product[sizeof(trans.product) - 1] = '\0';

        // Parse Category
        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(trans.category, token, sizeof(trans.category) - 1);
        trans.category[sizeof(trans.category) - 1] = '\0';

        // Parse Price
        if (!(token = strtok(nullptr, ","))) continue;
        trans.price = atof(token);

        // Parse Date
        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(dateStr, token, sizeof(dateStr) - 1);
        dateStr[sizeof(dateStr) - 1] = '\0';
        trans.date = LinkedList::convertDate(dateStr);

        // Parse Payment Method
        if (!(token = strtok(nullptr, ","))) continue;
        strncpy(trans.paymentMethod, token, sizeof(trans.paymentMethod) - 1);
        trans.paymentMethod[sizeof(trans.paymentMethod) - 1] = '\0';

        // Only add transactions with valid dates
        if (trans.date > 0) {
            transactions.insert(trans);
        }
    }
    file.close();

    // Sort transactions by date
    transactions.mergeSort();

    // Search for Electronics purchases with Credit Card
    auto [count, percentage] = transactions.searchElectronicsCreditCard();

    // Display results
    std::cout << "Total number of transactions: " << transactions.getSize() << std::endl;
    std::cout << "Electronics purchases with Credit Card: " << count << std::endl;
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) 
              << percentage << "%" << std::endl;
    
    std::cout << "\nSorted transactions by date:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    transactions.display();

    return 0;
}