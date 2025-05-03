#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <type_traits>

struct Transaction {
    std::string customerID;
    std::string product;
    double price;
    std::string date; // yyyy-mm-dd
    std::string category;
    std::string paymentMethod;

    // convert date string to tm structure for comparison
    std::tm getDateAsTm() const {
        std::tm time = {};
        std::istringstream ss(date);
        ss >> std::get_time(&time, "%Y-%m-%d");
        return time;
    }

    void print() const {
        std::cout << "Customer ID: " << customerID
                  << ", Product: " << product
                  << ", Price: $" << price
                  << ", Date: " << date
                  << ", Category: " << category
                  << ", Payment: " << paymentMethod << std::endl;
    }
};

struct Review {
    std::string productID;
    std::string customerID;
    int rating;
    std::string reviewText;

    void print() const {
        std::cout << "Product ID: " << productID
                  << ", Customer ID: " << customerID
                  << ", Rating: " << rating
                  << ", Review: " << reviewText << std::endl;
    }
};

struct WordFrequency {
    std::string word;
    int frequency;

    WordFrequency(const std::string& w, int f) : word(w), frequency(f) {}
};

// node
template <typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

// linked list
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() {
        clear();
    }

    void append(const T& value) {
        Node<T>* newNode = new Node<T>(value);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        size++;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // element at a specific index
    T& get(int index) {
        if (index < 0 || index >= size) throw std::out_of_range("Index out of bounds");

        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    }

    // merge sort implementation for linked list
    void mergeSort() {
        head = mergeSortRecursive(head);

        // update tail after sorting
        if (head != nullptr) {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            tail = current;
        }
    }

private:
    // recursive merge sort helper function
    Node<T>* mergeSortRecursive(Node<T>* node) {
        // base case: if list is empty or has only one element
        if (node == nullptr || node->next == nullptr) return node;

        // split the list into two halves
        Node<T>* middle = getMiddle(node);
        Node<T>* nextOfMiddle = middle->next;
        middle->next = nullptr;

        // recursively sort both halves
        Node<T>* left = mergeSortRecursive(node);
        Node<T>* right = mergeSortRecursive(nextOfMiddle);

        // merge the sorted halves
        return merge(left, right);
    }

    // find the middle node of a linked list
    Node<T>* getMiddle(Node<T>* head) {
        if (head == nullptr) return head;

        Node<T>* slow = head;
        Node<T>* fast = head;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }

    // merge two sorted linked lists
    Node<T>* merge(Node<T>* a, Node<T>* b) {
        Node<T>* result = nullptr;

        // base cases
        if (a == nullptr) return b;
        if (b == nullptr) return a;

        // compare
        if constexpr
        (std::is_same<T, Transaction>::value) {
            Transaction& transA = static_cast<Node<Transaction>*>(a)->data;
            Transaction& transB = static_cast<Node<Transaction>*>(b)->data;

            std::tm timeA = transA.getDateAsTm();
            std::tm timeB = transB.getDateAsTm();

            time_t dateA = std::mktime(&timeA);
            time_t dateB = std::mktime(&timeB);

            if (dateA <= dateB) {
                result = a;
                result->next = merge(a->next, b);
            } else {
                result = b;
                result->next = merge(a, b->next);
            }
        }

        else if constexpr 
        (std::is_same<T, WordFrequency>::value) {
            WordFrequency& wfA = static_cast<Node<WordFrequency>*>(a)->data;
            WordFrequency& wfB = static_cast<Node<WordFrequency>*>(b)->data;

            if (wfA.frequency >= wfB.frequency) {
                result = a;
                result->next = merge(a->next, b);
            } else {
                result = b;
                result->next = merge(a, b->next);
            }
        }

        // TODO implement for other data types

        else {
            result = a;
            result->next = merge(a->next, b);
        }

        return result;
    }

public:
    LinkedList<Transaction> searchByCategory(const std::string& category) {
        LinkedList<Transaction> results;
        Node<Transaction>* current = static_cast<Node<Transaction>*>(head);

        while (current != nullptr) {
            if (current->data.category == category) {
                results.append(current->data);
            }
            current = current->next;
        }

        return results;
    }

    LinkedList<Transaction> searchByPaymentMethod(const std::string& paymentMethod) {
        LinkedList<Transaction> results;
        Node<Transaction>* current = static_cast<Node<Transaction>*>(head);

        while (current != nullptr) {
            if (current->data.paymentMethod == paymentMethod) {
                results.append(current->data);
            }
            current = current->next;
        }

        return results;
    }

    LinkedList<Transaction> searchByCategoryAndPayment(const std::string& category, const std::string& paymentMethod) {
        LinkedList<Transaction> results;
        Node<Transaction>* current = static_cast<Node<Transaction>*>(head);

        while (current != nullptr) {
            if (current->data.category == category && current->data.paymentMethod == paymentMethod) {
                results.append(current->data);
            }
            current = current->next;
        }

        return results;
    }

    LinkedList<Review> searchByRating(int rating) {
        LinkedList<Review> results;
        Node<Review>* current = static_cast<Node<Review>*>(head);

        while (current != nullptr) {
            if (current->data.rating == rating) {
                results.append(current->data);
            }
            current = current->next;
        }

        return results;
    }

    WordFrequency* getWord(const std::string& word) {
        Node<WordFrequency>* current = static_cast<Node<WordFrequency>*>(head);

        while (current != nullptr) {
            if (current->data.word == word) {
              return &current->data;
            }
            current = current->next;
        }

        return nullptr;
    }

    // helper method to print all elements
    void printAll() {
        Node<T>* current = head;

        while (current != nullptr) {
            current->data.print();
            current = current->next;
        }
        std::cout << "\n";
    }

    // iterator class for linkedlist
    class Iterator {
    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* start) : current(start) {}

        bool hasNext() const {
            return current != nullptr;
        }

        T& next() {
            if (!hasNext()) {
                throw std::out_of_range("No more elements");
            }

            T& data = current->data;
            current = current->next;
            return data;
        }
    };

    // get iterator starting from head
    Iterator iterator() const {
        return Iterator(head);
    }
};

#endif
