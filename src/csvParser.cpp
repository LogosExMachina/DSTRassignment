#include "csvParser.h"

CSVParser::CSVParser() {}
CSVParser::~CSVParser() {}

// Loads the contents of a csv file at 'filePath',
// returns a DataTable containing its deserialized representation.
DataTable CSVParser::parseCSV(std::string filePath, ColumnType colTypes[], int stringLengths[], int nColTypes) {

    DataTable output;

    // Open the file handle
    std::ifstream fileHandle(filePath);

    if(!fileHandle.is_open()) {
        // Replace this by some error code thingy...
        std::cerr << "Error opening the file!";
        return output;
    }

    std::string row;
    int nRows=0, nCols=0;

    // Read the first row first
    std::getline(fileHandle, row);

    // Get the amount of actual columns first
    for(int c=0;c<row.size();c++) {
        if(
            ','==row[c] ||      // Char is a comma OR ...
            row.size()-1==c     // its the last character ...
        )
        nCols++;
    }

    // Error, differing amount of columns
    if(nCols != nColTypes) {
        // Replace this by some error code thingy...
        std::cout << "Error, differing amount of columns!" << std::endl;
        return output;
    }

    bool hasStrings = false;

    // Determine the byte size of each row
    int rowByteSize=0;
    for(int i=0;i<nCols;i++) {
        switch(colTypes[i]) {
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
                    (NULL==stringLengths)?
                    0 : (int)stringLengths[i];
                    //sizeof(std::string);
                break;
            case ColumnType::BOOL:
                rowByteSize+=(int)sizeof(bool);
                break;
            default: break;
        }
    }

    if(hasStrings) {
        if(NULL==stringLengths) {
            // Replace this by some error code thingy...
            std::cout << "Invalid string lengths array!" << std::endl;
            return output;
        }
    }

    // Initialize the column name array
    std::string *colNames = new std::string[nColTypes];
    output.setColumnNames(colNames);

    // Get the name of each column
    std::stringstream ssbuffer;
    std::string colName;
    int colCounter = 0;
    for(int c=0;c<row.size();c++) {
        if(
            ','==row[c] ||      // Char is a comma OR ...
            row.size()-1==c     // its the last character ...
        ) {
            // Make sure the last char is included
            if(row.size()-1==c && ','!=row[c]) ssbuffer << row[c];

            colName = ssbuffer.str();
            // Assign the column name
            output.setColumnName(colCounter, colName);
            std::cout << "Col name = " << colName << std::endl;

            ssbuffer = std::stringstream(); // Reset buffer
            colCounter++;
        } else {
            ssbuffer << row[c]; // Push char to the buffer
        }
    }

    // Calculate the number of remaining rows
    while(std::getline(fileHandle, row)) nRows++;

    std::cout << "nRows = " << nRows << std::endl;
    std::cout << "rowByteSize = " << rowByteSize << std::endl;

    // Allocate the data buffer
    output.dataBuffer = new unsigned char[rowByteSize*nRows];
    for(int i=0;i<nRows;i++) {
        for(int j=0;j<rowByteSize;j++) {
            output.dataBuffer[(i*rowByteSize) + j]=0x00;
        }
    }

    // Assign the respective data to the output table

    output.nCols=nCols;
    output.nRows=nRows;
    output.rowByteSize=rowByteSize;

    output.stringLengths = new int[nCols];
    memcpy(output.stringLengths, stringLengths, nCols*sizeof(int));

    output.columnTypes = new ColumnType[nColTypes];
    memcpy(output.columnTypes, colTypes, nColTypes*sizeof(ColumnType));

    // Reset the position (internal pointers of the
    // file handle)
    fileHandle.clear();
    fileHandle.seekg(0);

    // Advance once to skip the names row
    std::getline(fileHandle, row);

    // Iterate through each remaining row, and deserialize
    // the data into the buffer.
    int rowCounter = 0;
    colCounter=0;
    //std::string colContent;
    int colOffsetBytes=0;
    ssbuffer = std::stringstream(); // Reset buffer
    while(std::getline(fileHandle, row)) {

        std::cout << "> Row parse iteration" << std::endl;

        colCounter=0;
        colOffsetBytes=0;
        // Iterate through each character
        for(int c=0;c<row.size();c++) {
            if(
                ','==row[c] ||      // Char is a comma OR ...
                row.size()-1==c     // its the last character ...
            ) {
                // Make sure the last char is included
                if(row.size()-1==c && ','!=row[c]) ssbuffer << row[c];

                // DEBUGGING, maybe do under verbose mode later
                std::cout << "Buffer contents: " <<
                ssbuffer.str() << std::endl;

                // Anything written in the csv after the number of
                // columns is ignored
                if(colCounter >= nCols) break;

                switch (colTypes[colCounter])
                {
                case ColumnType::INT: {
                    int parsedInt = std::stoi(ssbuffer.str());

                    output.setIntAt(colCounter, rowCounter, parsedInt);

                    /*
                    memcpy(
                        output.getColRowBufferPtr(colCounter, rowCounter),
                        (unsigned char*)&parsedInt,
                        sizeof(int));
                    */

                    // DEBUGGING, maybe do under verbose mode later
                    std::cout << "Parsed int at [col=" << colCounter << ";row="
                    << rowCounter  << "] = "<< parsedInt << std::endl;

                    colOffsetBytes+=sizeof(int);
                }
                break;
                case ColumnType::DOUBLE: {
                    double parsedDouble = (ssbuffer.str().empty())?
                        0 : std::stod(ssbuffer.str());

                    output.setDoubleAt(colCounter, rowCounter, parsedDouble);

                    // DEBUGGING, maybe do under verbose mode later
                    std::cout << "Parsed double at [col=" << colCounter << ";row="
                    << rowCounter  << "] = "<< parsedDouble << std::endl;

                    colOffsetBytes+=sizeof(double);
                }
                break;
                case ColumnType::FLOAT: {
                    float parsedFloat = std::stof(ssbuffer.str());

                    output.setFloatAt(colCounter, rowCounter, parsedFloat);

                    // DEBUGGING, maybe do under verbose mode later
                    std::cout << "Parsed float at [col=" << colCounter << ";row="
                    << rowCounter  << "] = "<< parsedFloat << std::endl;

                    colOffsetBytes+=sizeof(float);
                }
                break;
                case ColumnType::STRING: {
                    // Error, cannot be a null-length string
                    if(stringLengths[colCounter]<=0) {
                        std::cout << "Invalid string length at col " <<
                        colCounter << "!" << std::endl;
                        break;
                    }

                    std::string parsedString = ssbuffer.str();
                    trimLeadingInPlace_STL(parsedString);
                    trimTrailingInPlace_STL(parsedString);
                    const char* stringContents = parsedString.c_str();

                    output.setStringAt(colCounter, rowCounter, stringContents);

                    // DEBUGGING, maybe do under verbose mode later
                    std::cout << "Parsed string at [col=" << colCounter << ";row="
                    << rowCounter  << "] = "<< parsedString.c_str() << std::endl;

                    colOffsetBytes+=stringLengths[colCounter]; //sizeof(std::string);
                }
                break;
                case ColumnType::BOOL: {

                    std::string content = ssbuffer.str();
                    trimLeadingInPlace_STL(content);
                    trimTrailingInPlace_STL(content);

                    bool parsedBool = content=="True" || content=="true";
                    output.setBoolAt(colCounter, rowCounter, parsedBool);

                    // DEBUGGING, maybe do under verbose mode later
                    std::cout << "Parsed bool at [col=" << colCounter << ";row="
                    << rowCounter  << "] = "<< parsedBool << std::endl;
                    colOffsetBytes+=sizeof(bool);
                }
                break;
                default: break;
                }

                ssbuffer = std::stringstream("");
                colCounter++;

            } else {
                ssbuffer << row[c]; // Push char to buffer
            }
        }

        rowCounter++;
    }

    // Close the file handle
    fileHandle.close();

    return output;
}

// Saves/serializes the contents of 'table' at the
// file given by 'filePath'
bool CSVParser::saveCSV(std::string filePath, DataTable table) {

    // Validate the Data Table first
    if(!table.wasInitialized()) {
        return false;
    }

    // Open the file handle
    std::ofstream fileHandle(filePath);

    if(!fileHandle.is_open()) {
        std::cerr << "Error opening the file!";
        return false;
    }

    // Serialize the column names
    for(int i=0; i<table.nCols; i++) {
        fileHandle << table.columnNames[i];
        // Add comma separator (except if its the last column)
        if(table.nCols-1!=i) fileHandle << ",";
    }
    fileHandle << "\n"; // go to next line

    // Serialize each value
    for(int i=0; i<table.nRows; i++) {
        for(int j=0; j<table.nCols; j++) {
            switch(table.columnTypes[j]) {
                case ColumnType::INT: {
                    int intVal = table.getIntAt(j,i);//*((int*)table.getRowColBufferPtr(j, i));
                    fileHandle << intVal;
                } break;
                case ColumnType::DOUBLE: {
                    double doubleVal = table.getDoubleAt(j,i);//*((double*)table.getRowColBufferPtr(j, i));
                    fileHandle << doubleVal;
                } break;
                case ColumnType::FLOAT: {
                    float floatVal = table.getFloatAt(j,i);//*((float*)table.getRowColBufferPtr(j, i));
                    fileHandle << floatVal;
                } break;
                case ColumnType::STRING: {
                    std::string strVal = table.getStringAt(j,i);
                    fileHandle << strVal;
                } break;
                case ColumnType::BOOL: {
                    fileHandle << table.getBoolAt(j,i);
                    //(*((bool*)table.getRowColBufferPtr(j, i)))? "True" : "False";
                } break;
                default: break;
            }
            // Add comma separator (except if its the last column)
            if(table.nCols-1!=j) fileHandle << ",";
        }
        fileHandle << "\n"; // go to next line
    }

    // Close the file handle
    fileHandle.close();
    return true; // Successfuly saved
}


