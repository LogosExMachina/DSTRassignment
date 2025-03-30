#include "dataTable.h"

DataTable::DataTable() {}

DataTable::~DataTable() {
    if(NULL!=columnNames) {
        delete columnNames;
    }

    if(NULL!=dataBuffer) {
        delete dataBuffer;
    }
}

int DataTable::getIntAt(int colNum, int rowNum) {

    if(checkLookupError(colNum, rowNum, ColumnType::INT)) 
        return -1;

    int target;
    memcpy(
        &target, 
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
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
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
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
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
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

    std::string target;
    memcpy(
        &target, 
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        getColumnByteDepth(colNum)
    );

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
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        getColumnByteDepth(colNum)
    );

    return target;
}

bool DataTable::getBoolAt(std::string colName, int rowNum) {

}

// Numbering starts from 0
bool DataTable::setIntAt(int colNum, int rowNum, int newInt) {
    if(checkLookupError(colNum, rowNum, ColumnType::INT)) 
        return -1;

    int newIntCpy = newInt; // force into modifiable lvalue
    memcpy(
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        &newIntCpy,
        getColumnByteDepth(colNum)
    );
}

bool DataTable::setIntAt(std::string colName, int rowNum, int newInt) {


}

// Numbering starts from 0
bool DataTable::setDoubleAt(int colNum, int rowNum, double newDouble) {
    if(checkLookupError(colNum, rowNum, ColumnType::DOUBLE)) 
        return -1;

    double newDoubleCpy = newDouble; // force into modifiable lvalue
    memcpy(
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        &newDoubleCpy,
        getColumnByteDepth(colNum)
    );
}

bool DataTable::setDoubleAt(std::string colName, int rowNum, double newDouble) {

}

// Numbering starts from 0
bool DataTable::setFloatAt(int colNum, int rowNum, float newFloat) {
    if(checkLookupError(colNum, rowNum, ColumnType::FLOAT)) 
        return -1;

    float newFloatCpy = newFloat; // force into modifiable lvalue
    memcpy(
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        &newFloatCpy,
        getColumnByteDepth(colNum)
    );
}

bool DataTable::setFloatAt(std::string colName, int rowNum, float newFloat) {

}

// Numbering starts from 0
bool DataTable::setStringAt(int colNum, int rowNum, std::string newString) {
    if(checkLookupError(colNum, rowNum, ColumnType::STRING)) 
        return -1;

    std::string newStringCpy = newString; // force into modifiable lvalue
    memcpy(
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        &newStringCpy,
        getColumnByteDepth(colNum)
    );
}

bool DataTable::setStringAt(std::string colName, int rowNum, std::string newString) {

}

// Numbering starts from 0
bool DataTable::setBoolAt(int colNum, int rowNum, bool newBool) {
    if(checkLookupError(colNum, rowNum, ColumnType::STRING)) 
        return -1;

    bool newBoolCpy = newBool; // force into modifiable lvalue
    memcpy(
        (unsigned char*)dataBuffer 
        + (rowByteSize*rowNum) 
        +  getColumnByteOffset(colNum),
        &newBoolCpy,
        getColumnByteDepth(colNum)
    );
}

bool DataTable::setBoolAt(std::string colName, int rowNum, bool newBool) {

}

std::string DataTable::print() {

    // Print column names first
    std::cout << "| ";
    for(int i=0;i<nCols;i++) std::cout << columnNames[i] << " | ";
    std::cout << std::endl;

    // Print the actual data
    for(int i=0;i<nRows;i++) {
        std::cout << "| ";
        for(int j=0;j<nCols;j++) {
            switch(columnTypes[j]) {
                case ColumnType::INT: 
                    std::cout << 
                    *((int*)
                    dataBuffer +
                    getColumnByteOffset(j));    
                break;
                case ColumnType::DOUBLE: 
                    std::cout << 
                    *((double*)
                    dataBuffer +
                    getColumnByteOffset(j));
                break;
                case ColumnType::FLOAT:
                    std::cout << 
                    *((float*)
                    dataBuffer +
                    getColumnByteOffset(j)); 
                break;
                case ColumnType::STRING:
                    std::cout << 
                    *((std::string*)
                    dataBuffer +
                    getColumnByteOffset(j))->c_str(); 
                break;
                case ColumnType::BOOL: 
                    std::cout << 
                    *((bool*)
                    dataBuffer +
                    getColumnByteOffset(j));
                break;
                default: break;
            }
            std::cout << " | ";
        }
        std::cout << std::endl; 
    }
}

void DataTable::printRowBinary(int nRow) {
    unsigned char* rowBuffer = new unsigned char[rowByteSize];    
    memcpy(
        rowBuffer, 
        dataBuffer 
        + (size_t)(nRow*rowByteSize),
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
    this->columnNames=newColumnNames;
}

// Returns true if successful, false otherwise.
// Numbering starts from 0
bool DataTable::setColumnName(int col, std::string newColumnName) {
    this->columnNames[col] = newColumnName;
}

bool DataTable::wasInitialized() {
    return 
        this->nRows > 0 &&
        this->nCols > 0 &&
        this->columnNames != NULL &&
        this->columnTypes != NULL && 
        this->dataBuffer != NULL &&
        this->rowByteSize != -1; 
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
            depth=sizeof(std::string);    
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
    for(int i=0;i<nCol;i++) {
        int nColBytes;
        switch(columnTypes[i]) {
            case ColumnType::INT: 
                nColBytes=sizeof(int);
                break;
            case ColumnType::DOUBLE: 
                nColBytes=sizeof(double);
                break;
            case ColumnType::FLOAT:
                nColBytes=sizeof(float);
                break;
            case ColumnType::STRING:
                nColBytes=sizeof(std::string);
                break;
            case ColumnType::BOOL:
                nColBytes=sizeof(bool);
                break;
            default: break;
        }
        colOffset+=nColBytes;
    }

    return colOffset;
}

// Returns true if an error occurred, false otherwise
bool DataTable::checkLookupError(int colNum, int rowNum, ColumnType colType) {
    bool output = false;

    if(!wasInitialized()) {
        errormask |= (unsigned char)DataTableError::NOT_INITIALIZED;        
        output = true;
    }

    if(colNum>nCols || rowNum>nRows) {
        errormask |= (unsigned char)DataTableError::OUT_OF_BOUNDS;
        output = true;
    }
    
    if(colType!=columnTypes[colNum]) {
        errormask |= (unsigned char)DataTableError::INVALID_TYPE_ACCESS;
        output = true;
    }
    return output;
}

