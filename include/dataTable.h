#pragma once 

#ifndef DATATABLE_H
#define DATATABLE_H

#include <iostream>
#include <memory.h>
#include <bitset>
#include <string>

enum class ColumnType {
    INT,
    DOUBLE, 
    FLOAT, 
    STRING,
    BOOL
};

enum class DataTableError {
    // Self explanatory
    NOT_INITIALIZED = 1,
    // Self explanatory
    OUT_OF_BOUNDS, 
    // i.e. trying to get a double from an
    // int column  
    INVALID_TYPE_ACCESS,
    // The provided strinLengths array wasnt 
    // initialized or is incorrect
    //INVALID_STRING_LENGTHS_ARR, (?)
};

class DataTable {

    // Declare CSVParser as friend so it does not need 
    // getters/setters for low-level data. 
    // (We don't need dumb OOP boilerplate, thats why)
    friend class CSVParser;

public:

    DataTable();
    ~DataTable();

    // Numbering starts from 0
    int getIntAt(int colNum, int rowNum);
    int getIntAt(std::string colName, int rowNum);

    // Numbering starts from 0
    double getDoubleAt(int colNum, int rowNum);
    double getDoubleAt(std::string colName, int rowNum);

    // Numbering starts from 0
    float getFloatAt(int colNum, int rowNum);
    float getFloatAt(std::string colName, int rowNum);

    // Numbering starts from 0
    std::string getStringAt(int colNum, int rowNum);
    std::string getStringAt(std::string colName, int rowNum);

    // Numbering starts from 0
    bool getBoolAt(int colNum, int rowNum);
    bool getBoolAt(std::string colName, int rowNum);

    // Numbering starts from 0
    bool setIntAt(int colNum, int rowNum, int newInt);
    bool setIntAt(std::string colName, int rowNum, int newInt);

    // Numbering starts from 0
    bool setDoubleAt(int colNum, int rowNum, double newDouble);
    bool setDoubleAt(std::string colName, int rowNum, double newDouble);

    // Numbering starts from 0
    bool setFloatAt(int colNum, int rowNum, float newFloat);
    bool setFloatAt(std::string colName, int rowNum, float newFloat);

    // Numbering starts from 0
    bool setStringAt(int colNum, int rowNum, std::string newString);
    bool setStringAt(std::string colName, int rowNum, std::string newString);

    // Numbering starts from 0
    bool setBoolAt(int colNum, int rowNum, bool newBool);
    bool setBoolAt(std::string colName, int rowNum, bool newBool);

    std::string print();

    int getnRows();
    int getnCols();

    void printRowBinary(int nRow);

    std::string *getColumnNames();

    std::string getColumnName(int col);

    // Returns true if successful, false otherwise.
    bool setColumnNames(std::string *newColumnNames);

    // Returns true if successful, false otherwise.
    // Numbering starts from 0
    bool setColumnName(int col, std::string newColumnName);

    bool wasInitialized();

private:
    // (-1 and NULL for uninitialized values)

    // The 'data buffer' is just a buffer of bytes where all the actual 
    // data is stored. (Its unsigned because its values go from 0 to 255).
    // The data 'polymorphism' is done through the use of
    // pointers and type casting.
    unsigned char* dataBuffer=NULL;
    int rowByteSize=-1; // The size in bytes of a row in the buffer
    int nRows=-1;
    int nCols=-1; 
    unsigned char errormask = 0x0; // Use this to track errors
    std::string *columnNames;

    // String lengths for each column are needed since 
    // strings vary in (byte) depth.
    // If the column is not a string, set its respective 
    // value to -1.
    int *stringLengths=NULL; 
    
    // The type for each column
    ColumnType *columnTypes=NULL;
    
    int getColumnByteDepth(int nCol);
    int getColumnByteOffset(int nCol);

    // Returns the address in the data buffer where
    // the value of [colNum, rowNum] is stored
    unsigned char* getColRowBufferPtr(int colNum, int rowNum);

    // Returns true if an error occurred when retrieving(getting) data, false otherwise
    bool checkLookupError(int colNum, int rowNum, ColumnType colType);

    // Returns true if an error occurred when modifying(setting) data, false otherwise
    bool checkModificationError(int colNum, int rowNum, ColumnType colType);


};

#endif // DATATABLE_H