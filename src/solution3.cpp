#include <iostream>
#include "dataTable.h"
#include "csvParser.h"
#include "dynamicArray.hpp"
#include "dynamicArray_unsortable.hpp"
#include "date.hpp"
#include "strUtils.h"

// Solution 3: Which words are most frequently used in product 
// reviews rated 1-star? 
// (Hint: Extract the most common words, sort them by 
// frequency, and present the results.)

CSVParser parser;
DataTable rawReviews, filteredReviews;
const int RATING_COLUMN_INDEX = 2;
const int REVIEWTEXT_COLUMN_INDEX = 3;

// The following two are pointers given that
// declaring template classes as globals leads to weird
// undefined behaviour. Hence, these are instantiated locally,
// and de-referenced through these global pointers
DynamicArray_Unsortable<std::string> *wordAtlasPtr;
DynamicArray_Sortable<int> *frequencyArrayPtr;


// STEP 1: Load the review entries
void loadReviews() {

    parser.setVerbose(true);

    ColumnType columnTypes[] = {
        ColumnType::STRING, // Product ID
        ColumnType::STRING, // Customer ID
        ColumnType::INT, // Rating
        ColumnType::STRING, // Review Text 
    };
    
    int stringLengths[] = {
        16, // Product ID
        16, // Customer ID
        -1, // Rating
        92, // Review Text 
    };
    
    int nCols = sizeof(columnTypes)/sizeof(ColumnType);
    
    rawReviews
    = parser.parseCSV(
        "data\\reviews_cleaned.csv", 
        //"data\\reviews_cleaned.csv",
        columnTypes,
        stringLengths,
        nCols,
        //100
        -1 // Load ALL entries 
    );

    std::cout << 
        (rawReviews.wasInitialized()? 
        "> rawReviews sucessfully initialized":
        "> Error when initializing rawReviews!") 
    << std::endl;

}

// STEP 2: Filter the reviews with 1-star rating
void filterReviews() {

    DynamicArray_Sortable<int> filteredIDs = DynamicArray_Sortable<int>();

    // First extract the ids
    for(int i=0; i<rawReviews.getnRows(); i++) 
    {
        // If the review has a 1 star rating
        if(1 == rawReviews.getIntAt(RATING_COLUMN_INDEX, i)) {
            filteredIDs.pushBack(i); 
        }
    }

    // Create and allocate the filtered table
    filteredReviews = DataTable();
    bool filteredTableAllocated = filteredReviews.reallocate(filteredIDs.getSize(), rawReviews.getnCols(), 
    rawReviews.getColumnTypes(), rawReviews.getStringLengths());

    filteredReviews.setColumnName(0, "Product ID");
    filteredReviews.setColumnName(1, "Customer ID");
    filteredReviews.setColumnName(2, "Rating");
    filteredReviews.setColumnName(3, "Review Text");

    if(filteredTableAllocated)
        std::cout << "> Filtered Table allocated" <<std::endl;
    else {
        std::cout << "> Filtered Table failed to allocate..." << std::endl;
        return;
    }

    // Then use the filtered IDs to extract the specific 
    // review entries into the new table
    for(int i=0; i<filteredIDs.getSize(); i++) {

        // Set Product ID
        filteredReviews.setStringAt(0, i,
            rawReviews.getStringAt(0, filteredIDs.getAt(i)));

        // Set Customer ID
        filteredReviews.setStringAt(1, i,
            rawReviews.getStringAt(1, filteredIDs.getAt(i)));
        
        // Set Rating
        filteredReviews.setIntAt(2, i,
            rawReviews.getIntAt(2, filteredIDs.getAt(i)));
        
        // Set Review text
        filteredReviews.setStringAt(3, i,
            rawReviews.getStringAt(3, filteredIDs.getAt(i)));
    }

    std::cout << "> Filtered Table loaded" << std::endl;
    filteredReviews.print();
}

// Utility for tokenization in step 3 and 4
// Warning: Make sure to manually deallocate the output array
DynamicArray_Unsortable<std::string> tokenizeFromWhitespace(std::string txt) {
    
    DynamicArray_Unsortable<std::string> tokenArray = 
        DynamicArray_Unsortable<std::string>();
    tokenArray.setAutoDealloc(false);

    std::stringstream ssbuffer;
    
    // (Manually) Tokenize the text
    for(int c=0; c<txt.size(); c++) {
        if(
            ' ' == txt[c] || // Whitespace found OR...
            txt.size()-1==c // Its the last character...
        )
        {
            std::cout << "> Buffer contents = " << ssbuffer.str() << std::endl;

            std::string bufferstr = std::string(ssbuffer.str());
            if(!(bufferstr.empty())) {
                tokenArray.pushBack(bufferstr);
            }
            ssbuffer = std::stringstream("");// Reset buffer
        }
        else {
            if('\"' != txt[c] && ',' != txt[c])
                ssbuffer << txt[c];
        }
    }

    return tokenArray;
}

// STEP 3: Create a 'word atlas' from the filtered reviews, and calculate
// word frequencies
void generateWordAtlas() {
    
    wordAtlasPtr =  new DynamicArray_Unsortable<std::string>();
    DynamicArray_Unsortable<std::string> &wordAtlas = *wordAtlasPtr;
    //wordAtlas.setVerbose(true);
    wordAtlas.setAutoDealloc(false);
    
    frequencyArrayPtr = new DynamicArray_Sortable<int>();
    DynamicArray_Sortable<int> &frequencyArray = *frequencyArrayPtr;
    //frequencyArray.setVerbose(true);
    frequencyArray.setAutoDealloc(false);

    std::cout << "> Generating Word Atlas..." << std::endl;

    for(int i=0; i<filteredReviews.getnRows(); i++) {
        std::string reviewText = 
        filteredReviews.getStringAt(REVIEWTEXT_COLUMN_INDEX, i);
        
        DynamicArray_Unsortable<std::string> tokenArray = tokenizeFromWhitespace(reviewText);

        // Search for each token in the word Atlas
        // -> If it does not exist, add it
        for(int t=0; t<tokenArray.getSize(); t++) {
            std::string token = tokenArray.getAt(t);
            
            if(token.empty()) continue;

            int atlasLookupResult = wordAtlas.linearSearch(token);
            if(-1 == atlasLookupResult) {
                
                std::cout << "\t>> Adding '" << token 
                    << "' to the Atlas" << std::endl;
                
                wordAtlas.pushBack(token);
                frequencyArray.pushBack(1); 

                //std::cout << "\t>> Token added" << std::endl;
            } else {
                // Increase frequency of the given word in the atlas
                int prevFrequency = frequencyArray.getAt(atlasLookupResult);
                frequencyArray.insertAt(prevFrequency+1, atlasLookupResult);
            }
        }

        tokenArray.free();  
    }

    std::cout << "> Word Atlas and Frequency Array generated" << std::endl;
    
}

// STEP 4: Sort the frequency array parallel to the word atlas
void sortByFrequencyArray() {
    DynamicArray_Unsortable<std::string>& wordAtlas = *wordAtlasPtr; 
    frequencyArrayPtr->quicksort<std::string>(0, frequencyArrayPtr->getSize()-1, wordAtlas);
}

// STEP 5: Choose the top 10 most common words and display them 
void displayTop10() {

    std::cout << "#### TOP 10 MOST FREQUENT WORDS ####" << std::endl;

    // Quicksort is ordering from smallest to largest.
    // Hence, do the loop from the end to the beginning
    // of the array.
    int topCounter=1;
    for(
        int i=frequencyArrayPtr->getSize()-1;
        i >= (frequencyArrayPtr->getSize()-1) - 10;
        i--
    ) {
        std::cout << "[ Top " << topCounter << " ]: \'" 
        << wordAtlasPtr->getAt(i) 
        << "\' ( Freq. = " << frequencyArrayPtr->getAt(i) << " )" 
        << std::endl;
        topCounter++;
    }
}

int main(int argc, char** argv) {

    loadReviews();
    //rawReviews.print();
    filterReviews();

    generateWordAtlas();

    std::cout << "> Word Atlas:\n" << 
    wordAtlasPtr->getAsString() << std::endl;
    std::cout << "> Frequency Array:\n" << 
    frequencyArrayPtr->getAsString() << std::endl;
    
    sortByFrequencyArray();

    std::cout << "> Word Atlas after sorting:\n" << 
    wordAtlasPtr->getAsString() << std::endl;
    std::cout << "> Frequency Array after sorting:\n" << 
    frequencyArrayPtr->getAsString() << std::endl;

    displayTop10();

    wordAtlasPtr->free();
    frequencyArrayPtr->free();
    delete wordAtlasPtr;
    delete frequencyArrayPtr;

    return 0;
}