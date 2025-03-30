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
};

class DataTable {

    // Declare CSVParser as friend so it does not need 
    // getters/setters for low-level data
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
    // -1 and NULL for uninitialized values
    unsigned char* dataBuffer=NULL;
    int rowByteSize=-1;
    int nRows=-1;
    int nCols=-1; 
    unsigned char errormask = 0x0;
    std::string *columnNames;
    ColumnType columnTypes[];
  
    int getColumnByteDepth(int nCol);
    int getColumnByteOffset(int nCol);

    // Returns true if an error occurred when retrieving(getting) data, false otherwise
    bool checkLookupError(int colNum, int rowNum, ColumnType colType);
    bool checkModificationError(int colNum, int rowNum, ColumnType colType);

};

#endif // DATATABLE_H