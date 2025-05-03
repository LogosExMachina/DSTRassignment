#include "shared/dataTable.h"

DataTable::DataTable() {

}

/*
// move constructor
DataTable::DataTable(DataTable&& other) noexcept {
    columnNames = other.columnNames;
    dataBuffer = other.dataBuffer;
    stringLengths = other.stringLengths;
    columnTypes = other.columnTypes;
    nCols = other.nCols;
    nRows = other.nRows;
    rowByteSize = other.rowByteSize;
    errormask = other.errormask;

    other.columnNames = nullptr;
    other.dataBuffer = nullptr;
    other.stringLengths = nullptr;
    other.columnTypes = nullptr;
    other.nCols = 0;
    other.nRows = 0;
    other.errormask = 0x0;
}

// move assignment
DataTable& DataTable::operator=(DataTable&& other) noexcept {
    if (this != &other) {
        columnNames = other.columnNames;
        dataBuffer = other.dataBuffer;
        stringLengths = other.stringLengths;
        columnTypes = other.columnTypes;
        nCols = other.nCols;
        nRows = other.nRows;
        errormask = other.errormask;

        delete[] columnNames;
        delete[] dataBuffer;
        delete[] stringLengths;
        delete[] columnTypes;

        other.columnNames = nullptr;
        other.dataBuffer = nullptr;
        other.stringLengths = nullptr;
        other.columnTypes = nullptr;
        other.nCols = 0;
        other.nRows = 0;
        other.errormask = 0x0;

    }
    return *this;
}
*/

void DataTable::free() {
    if(verbose)
        std::cout << "> Destroying DataTable..." << std::endl;

    if(columnNames) {
        if(verbose)
            std::cout << "\t> Destroying DataTable::columnNames..." << std::endl;
        delete[] columnNames;
    }

    if(dataBuffer) {
        if(verbose)
            std::cout << "\t> Destroying DataTable::dataBuffer..." << std::endl;
        delete[] dataBuffer;
    }

    if(stringLengths) {
        if(verbose)
            std::cout << "\t> Destroying DataTable::stringLengths..." << std::endl;
        delete[] stringLengths;
    }

    if(columnTypes) {
        if(verbose)
            std::cout << "\t> Destroying DataTable::columnTypes..." << std::endl;
        delete[] columnTypes;
    }
}

// Use this function to redefine the DataTable
// dimensions and memory size.
// WARNING: This will delete all the previous data stored.
bool DataTable::reallocate(int nRows, int nCols, ColumnType *columnTypes, int *stringLengths) {

    if(nRows < 0 || nCols < 0) return false;
    if(!columnTypes) return false;
    //if(!columnNames) return false;

    // Make sure to deallocate any previous data
    if(wasInitialized()) free();

    // Calculate row depth
    bool hasStrings = false;

    // Determine the byte size of each row
    int rowByteSize=0;
    for(int i=0;i<nCols;i++) {
        switch(columnTypes[i]) {
            case ColumnType::INT:
                rowByteSize+=(int)sizeof(int);
                break;
            case ColumnType::DOUBLE:
                rowByteSize+=(int)sizeof(double);
                break;
            case ColumnType::FLOAT:
                rowByteSize+=(int)sizeof(float);
                break;
            case ColumnType::STRING:
                hasStrings=true;
                rowByteSize+=
                    (stringLengths==nullptr)?
                    0 : (int)stringLengths[i];
                    //sizeof(std::string);
                break;
            case ColumnType::BOOL:
                rowByteSize+=(int)sizeof(bool);
                break;
            default: break;
        }
    }
    this->rowByteSize=rowByteSize;

    if(hasStrings) {
        if(NULL==stringLengths) {
            // Replace this by some error code thingy...
            std::cout << "Invalid string lengths array!" << std::endl;
            return false;
        }
    }

    // Initialize the column name array
    std::string *colNames = new std::string[nCols];
    for(int i=0; i<nCols; i++) {
        colNames[i] = std::string("");
        // Set this name by default
        colNames[i].append("ATTR");
        char conversionBuffer[16];
        snprintf(conversionBuffer, sizeof(conversionBuffer), "%d", i);
        colNames[i].append(conversionBuffer);
    }
    setColumnNames(colNames);

    // (Re)Allocate the data buffer
    dataBuffer = new unsigned char[rowByteSize*nRows];
    for(int i=0;i<nRows;i++) {
        for(int j=0;j<rowByteSize;j++) {
            dataBuffer[(i*rowByteSize) + j]=0x00;
        }
    }

    // Ressign the data to the table
    this->nCols=nCols;
    this->nRows=nRows;
    this->rowByteSize=rowByteSize;

    this->stringLengths = new int[nCols];
    memcpy(this->stringLengths, stringLengths, nCols*sizeof(int));

    this->columnTypes = new ColumnType[nCols];
    memcpy(this->columnTypes, columnTypes, nCols*sizeof(ColumnType));

    return true; // success
}

DataTable::~DataTable() {
}

int DataTable::getIntAt(int colNum, int rowNum) {

    if(checkLookupError(colNum, rowNum, ColumnType::INT))
        return -1;

    int target;
    memcpy(
        &target,
        getColRowBufferPtr(colNum, rowNum),
        getColumnByteDepth(colNum)
    );

    return target;
}

int DataTable::getIntAt(std::string colName, int rowNum) {

}

double DataTable::getDoubleAt(int colNum, int rowNum) {
    if(checkLookupError(colNum, rowNum, ColumnType::DOUBLE))
        return -1;

    double target;
    memcpy(
        &target,
        getColRowBufferPtr(colNum, rowNum),
        getColumnByteDepth(colNum)
    );

    return target;
}

double DataTable::getDoubleAt(std::string colName, int rowNum) {

}

float DataTable::getFloatAt(int colNum, int rowNum) {
    if(checkLookupError(colNum, rowNum, ColumnType::FLOAT))
        return -1;

    float target;
    memcpy(
        &target,
        getColRowBufferPtr(colNum, rowNum),
        getColumnByteDepth(colNum)
    );

    return target;
}

float DataTable::getFloatAt(std::string colName, int rowNum) {

}

// Numbering starts from 0
std::string DataTable::getStringAt(int colNum, int rowNum) {
    if(checkLookupError(colNum, rowNum, ColumnType::STRING))
        return nullptr;

    if(0>=stringLengths[colNum]) {
        return nullptr;
    }

    std::string target("");
    target.reserve(stringLengths[colNum]);
    char* targetContents = new char[stringLengths[colNum]];
    memcpy(
        targetContents,
        getColRowBufferPtr(colNum, rowNum),
        getColumnByteDepth(colNum)
    );
    target.append(targetContents);
    delete targetContents;

    return target;
}

std::string DataTable::getStringAt(std::string colName, int rowNum) {

}

// Numbering starts from 0
bool DataTable::getBoolAt(int colNum, int rowNum) {
    if(checkLookupError(colNum, rowNum, ColumnType::BOOL))
        return -1;

    bool target;
    memcpy(
        &target,
        getColRowBufferPtr(colNum, rowNum),
        getColumnByteDepth(colNum)
    );

    return target;
}

bool DataTable::getBoolAt(std::string colName, int rowNum) {

}

// Numbering starts from 0
bool DataTable::setIntAt(int colNum, int rowNum, int newInt) {
    if(checkModificationError(colNum, rowNum, ColumnType::INT))
        return false;

    int newIntCpy = newInt; // force into modifiable lvalue
    memcpy(
        getColRowBufferPtr(colNum, rowNum),
        &newIntCpy,
        getColumnByteDepth(colNum)
    );

    return true;
}

bool DataTable::setIntAt(std::string colName, int rowNum, int newInt) {

}

// Numbering starts from 0
bool DataTable::setDoubleAt(int colNum, int rowNum, double newDouble) {
    if(checkModificationError(colNum, rowNum, ColumnType::DOUBLE))
        return false;

    double newDoubleCpy = newDouble; // force into modifiable lvalue
    memcpy(
        getColRowBufferPtr(colNum, rowNum),
        &newDoubleCpy,
        getColumnByteDepth(colNum)
    );

    return true;
}

bool DataTable::setDoubleAt(std::string colName, int rowNum, double newDouble) {

}

// Numbering starts from 0
bool DataTable::setFloatAt(int colNum, int rowNum, float newFloat) {
    if(checkModificationError(colNum, rowNum, ColumnType::FLOAT))
        return false;

    float newFloatCpy = newFloat; // force into modifiable lvalue
    memcpy(
        getColRowBufferPtr(colNum, rowNum),
        &newFloatCpy,
        getColumnByteDepth(colNum)
    );

    return true;
}

bool DataTable::setFloatAt(std::string colName, int rowNum, float newFloat) {

}

// Numbering starts from 0
bool DataTable::setStringAt(int colNum, int rowNum, std::string newString) {
    if(checkModificationError(colNum, rowNum, ColumnType::STRING))
        return false;

    const char* newStringContents = newString.c_str();
    memcpy(
        getColRowBufferPtr(colNum, rowNum),
        newStringContents,
        getColumnByteDepth(colNum)
    );

    /*
    std::string newStringCpy = newString; // force into modifiable lvalue
    memcpy(
        getColRowBufferPtr(colNum, rowNum),
        &newStringCpy,
        getColumnByteDepth(colNum)
    );
    */

    return true;
}

bool DataTable::setStringAt(std::string colName, int rowNum, std::string newString) {

}

// Numbering starts from 0
bool DataTable::setBoolAt(int colNum, int rowNum, bool newBool) {
    if(checkModificationError(colNum, rowNum, ColumnType::BOOL))
        return false;

    bool newBoolCpy = newBool; // force into modifiable lvalue
    memcpy(
        getColRowBufferPtr(colNum, rowNum),
        &newBoolCpy,
        getColumnByteDepth(colNum)
    );

    return true;
}

bool DataTable::setBoolAt(std::string colName, int rowNum, bool newBool) {

}

void DataTable::print() {

    // Print column names first
    std::cout << "| ";
    for(int i=0;i<nCols;i++)
        std::cout <<
            //((columnNames[i].empty())? "NO_NAME": columnNames[i])
            columnNames[i]
            << " | ";
    std::cout << std::endl;

    if(!dataBuffer) {
        std::cout << "ERROR: DATABUFFER IS NULL" << std::endl;
        return;
    }

    // Print the actual data
    for(int i=0;i<nRows;i++) {
        std::cout << "| ";
        for(int j=0;j<nCols;j++) {
            switch(columnTypes[j]) {
                case ColumnType::INT:
                    std::cout << getIntAt(j, i);
                break;
                case ColumnType::DOUBLE:
                    std::cout << getDoubleAt(j, i);
                break;
                case ColumnType::FLOAT:
                    std::cout << getFloatAt(j, i);
                break;
                case ColumnType::STRING:
                    std::cout << getStringAt(j, i);
                break;
                case ColumnType::BOOL:
                    std::cout << (getBoolAt(j, i)? "True" : "False");
                break;
                default: break;
            }
            std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

int DataTable::getnRows() {
    return this->nRows;
}

int DataTable::getnCols() {
    return this->nCols;
}

void DataTable::printRowBinary(int nRow) {
    unsigned char* rowBuffer = new unsigned char[rowByteSize];
    memcpy(
        rowBuffer,
        (unsigned char*)dataBuffer +
        (size_t)(nRow*rowByteSize),
        (size_t)rowByteSize
    );

    std::cout<<"[r="<<nRow<<"]: ";
    for(int i=0;i<rowByteSize;i++) {
        char b = *(rowBuffer + i);
        std::bitset<8>byte(b);
        std::cout << byte << " ";
    }
    std::cout<<std::endl;

    delete rowBuffer;
}

std::string *DataTable::getColumnNames() {
    return this->columnNames;
}

std::string DataTable::getColumnName(int col) {
    return this->columnNames[col];
}

// Returns true if successful, false otherwise.
bool DataTable::setColumnNames(std::string *newColumnNames) {
    if(!newColumnNames) return false;

    this->columnNames=newColumnNames;
    return true;
}

// Returns true if successful, false otherwise.
// Numbering starts from 0
bool DataTable::setColumnName(int col, std::string newColumnName) {
    //if(col<0 || col>nCols-1) return false;

    this->columnNames[col] = std::string(newColumnName);
    return true;
}

ColumnType* DataTable::getColumnTypes() {
    return columnTypes;
}

int* DataTable::getStringLengths() {
    return stringLengths;
}

bool DataTable::wasInitialized() {
    return
        this->nRows > 0 &&
        this->nCols > 0 &&
        this->columnNames != NULL &&
        this->columnTypes != NULL &&
        this->dataBuffer != NULL &&
        this->rowByteSize != -1 &&
        this->stringLengths != NULL;
}

void DataTable::setVerbose(bool verbose) {
    this->verbose=verbose;
}

int DataTable::getColumnByteDepth(int nCol) {
    int depth=0;
    switch(columnTypes[nCol]) {
        case ColumnType::INT:
            depth=sizeof(int);
            break;
        case ColumnType::DOUBLE:
            depth=sizeof(double);
            break;
        case ColumnType::FLOAT:
            depth=sizeof(float);
            break;
        case ColumnType::STRING:
            depth = (stringLengths[nCol] > 0)? stringLengths[nCol] : 0;
            break;
        case ColumnType::BOOL:
            depth=sizeof(bool);
            break;
        default: break;
    }
    return depth;
}

int DataTable::getColumnByteOffset(int nCol) {
    int colOffset=0;
    for(int i=0;i<nCol;i++) colOffset+=getColumnByteDepth(i);
    return colOffset;
}

// Returns true if an error occurred, false otherwise
bool DataTable::checkLookupError(int colNum, int rowNum, ColumnType colType) {
    bool error = false;

    if(!wasInitialized()) {
        errormask |= (unsigned char)DataTableError::NOT_INITIALIZED;
        error = true;
    }

    if(colNum>nCols || rowNum>nRows) {
        errormask |= (unsigned char)DataTableError::OUT_OF_BOUNDS;
        error = true;
    }

    if(colType!=columnTypes[colNum]) {
        errormask |= (unsigned char)DataTableError::INVALID_TYPE_ACCESS;
        error = true;
    }
    return error;
}

// Returns true if an error occurred when modifying(setting) data, false otherwise
bool DataTable::checkModificationError(int colNum, int rowNum, ColumnType colType) {
    bool error = false;

    if(!wasInitialized()) {
        errormask |= (unsigned char)DataTableError::NOT_INITIALIZED;
        error = true;
    }

    if(colNum>nCols || rowNum>nRows) {
        errormask |= (unsigned char)DataTableError::OUT_OF_BOUNDS;
        error = true;
    }

    if(colType!=columnTypes[colNum]) {
        errormask |= (unsigned char)DataTableError::INVALID_TYPE_ACCESS;
        error = true;
    }
    return error;
}

// Returns the address in the data buffer where
// the value of [colNum, rowNum] is stored
unsigned char* DataTable::getColRowBufferPtr(int colNum, int rowNum) {
    return
        (unsigned char*)dataBuffer // Base buffer pointer
        + (size_t)(rowByteSize*rowNum) // Row (vertical) offset
        +  (size_t)getColumnByteOffset(colNum); // Column (horizontal) offset
}

