
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

// Constants
const int MAX_WORD_LENGTH = 50;
const int HASH_TABLE_SIZE = 997; // A prime number for better distribution
const int MAX_WORDS = 10000; // Maximum unique words we'll track

// Structure to store word and its frequency
struct WordFreq {
    char word[MAX_WORD_LENGTH];
    int frequency;
    
    WordFreq() {
        word[0] = '\0';
        frequency = 0;
    }
};

// Hash table entry with chaining
struct HashNode {
    WordFreq data;
    HashNode* next;
    
    HashNode() : next(nullptr) {}
};

// Custom string operations
size_t customStrlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

void customStrcpy(char* dest, const char* src, size_t maxLen) {
    size_t i;
    for (i = 0; i < maxLen - 1 && src[i]; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int customStrcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// Hash function
unsigned int hashString(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}

class WordFrequencyAnalyzer {
private:
    HashNode* hashTable[HASH_TABLE_SIZE];
    WordFreq* sortedWords;
    int uniqueWordCount;

    // Convert word to lowercase and remove punctuation
    void cleanWord(char* word) {
        char* write = word;
        for (char* read = word; *read; read++) {
            if (isalpha(*read)) {
                *write = tolower(*read);
                write++;
            }
        }
        *write = '\0';
    }

    // Insert or update word frequency in hash table
    void insertWord(const char* word) {
        if (word[0] == '\0') return;

        unsigned int index = hashString(word);
        HashNode* current = hashTable[index];

        // Search for existing word
        while (current) {
            if (customStrcmp(current->data.word, word) == 0) {
                current->data.frequency++;
                return;
            }
            current = current->next;
        }

        // Word not found, create new entry
        if (uniqueWordCount < MAX_WORDS) {
            HashNode* newNode = new HashNode();
            customStrcpy(newNode->data.word, word, MAX_WORD_LENGTH);
            newNode->data.frequency = 1;
            newNode->next = hashTable[index];
            hashTable[index] = newNode;
            uniqueWordCount++;
        }
    }

    // Collect words from hash table into array
    void collectWords() {
        sortedWords = new WordFreq[uniqueWordCount];
        int idx = 0;

        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            HashNode* current = hashTable[i];
            while (current) {
                customStrcpy(sortedWords[idx].word, current->data.word, MAX_WORD_LENGTH);
                sortedWords[idx].frequency = current->data.frequency;
                idx++;
                current = current->next;
            }
        }
    }

    // Counting sort based on frequency
    void countingSort() {
        // Find maximum frequency
        int maxFreq = 0;
        for (int i = 0; i < uniqueWordCount; i++) {
            if (sortedWords[i].frequency > maxFreq) {
                maxFreq = sortedWords[i].frequency;
            }
        }

        // Create count array and temporary array
        int* count = new int[maxFreq + 1]();
        WordFreq* temp = new WordFreq[uniqueWordCount];

        // Count frequencies
        for (int i = 0; i < uniqueWordCount; i++) {
            count[sortedWords[i].frequency]++;
        }

        // Calculate cumulative count
        for (int i = maxFreq - 1; i >= 0; i--) {
            count[i] += count[i + 1];
        }

        // Build output array
        for (int i = 0; i < uniqueWordCount; i++) {
            int pos = count[sortedWords[i].frequency] - 1;
            temp[pos] = sortedWords[i];
            count[sortedWords[i].frequency]--;
        }

        // Copy back to original array
        for (int i = 0; i < uniqueWordCount; i++) {
            sortedWords[i] = temp[i];
        }

        delete[] count;
        delete[] temp;
    }

public:
    WordFrequencyAnalyzer() : uniqueWordCount(0), sortedWords(nullptr) {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            hashTable[i] = nullptr;
        }
    }

    ~WordFrequencyAnalyzer() {
        // Clean up hash table
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            HashNode* current = hashTable[i];
            while (current) {
                HashNode* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] sortedWords;
    }

    void processReviews(const char* filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: Cannot open " << filename << std::endl;
            return;
        }

        char line[1024];
        bool firstLine = true;

        while (file.getline(line, sizeof(line))) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            char* token = strtok(line, ",");
            if (!token) continue; // Skip empty lines

            // Skip product ID
            token = strtok(nullptr, ",");
            if (!token) continue;

            // Get rating
            token = strtok(nullptr, ",");
            if (!token) continue;

            // Check if rating is 1
            int rating;
            try {
                rating = std::atoi(token);
            } catch (...) {
                continue;
            }

            if (rating != 1) continue;

            // Process review text
            token = strtok(nullptr, ",");
            if (!token) continue;

            // Tokenize review text into words
            char* word = strtok(token, " \t\n\r\f\v.,!?;:\"'()[]{}");
            while (word) {
                if (customStrlen(word) > 0) {
                    char cleanedWord[MAX_WORD_LENGTH];
                    customStrcpy(cleanedWord, word, MAX_WORD_LENGTH);
                    cleanWord(cleanedWord);
                    if (cleanedWord[0] != '\0') {
                        insertWord(cleanedWord);
                    }
                }
                word = strtok(nullptr, " \t\n\r\f\v.,!?;:\"'()[]{}");
            }
        }

        file.close();
    }

    void displayTopWords(int n) {
        if (uniqueWordCount == 0) {
            std::cout << "No words found in 1-star reviews." << std::endl;
            return;
        }

        // Collect and sort words
        collectWords();
        countingSort();

        // Display top n words (or all if less than n)
        std::cout << "\nMost frequent words in 1-star reviews:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        int displayCount = std::min(n, uniqueWordCount);
        for (int i = 0; i < displayCount; i++) {
            std::cout << sortedWords[i].word << ": " 
                     << sortedWords[i].frequency << " occurrences" << std::endl;
        }
    }

    int getUniqueWordCount() const {
        return uniqueWordCount;
    }
};

int main() {
    WordFrequencyAnalyzer analyzer;
    
    // Process reviews.csv
    analyzer.processReviews("data/reviews.csv");
    
    // Display statistics
    std::cout << "Total unique words found in 1-star reviews: " 
              << analyzer.getUniqueWordCount() << std::endl;
    
    // Display top 20 most frequent words
    analyzer.displayTopWords(20);

    return 0;
}