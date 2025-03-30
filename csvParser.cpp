#include "csvParser.h"

CSVParser::CSVParser() {}
CSVParser::~CSVParser() {}

DataTable CSVParser::parseCSV(std::string filePath, ColumnType colTypes[], int nColTypes) {

    DataTable output;

    // Open the file handle
    std::ifstream fileHandle(filePath);

    if(!fileHandle.is_open()) {
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
        std::cout << "Error, differing amount of columns!" << std::endl;
        return output;
    }
    
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
                rowByteSize+=(int)sizeof(std::string);
                break;
            case ColumnType::BOOL: 
                rowByteSize+=(int)sizeof(bool);
                break;
            default: break;    
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

                if(colCounter >= nCols) break;

                //colContent = ssbuffer.str();
                //unsigned char *conversionBuffer;

                switch (colTypes[colCounter])
                {
                case ColumnType::INT: {
                    //conversionBuffer = new unsigned char[sizeof(int)];
                    int parsedInt = std::stoi(ssbuffer.str());
                    memcpy(
                        output.dataBuffer + // Base position 
                        (rowCounter*rowByteSize) + // Row offset
                        (colOffsetBytes), // Column offset
                        &parsedInt, sizeof(int));
                    std::cout << "Parsed int at [col=" << colCounter << ";row=" 
                    << rowCounter  << "] = "<< parsedInt << std::endl;
                    colOffsetBytes+=sizeof(int);
                }
                break;
                case ColumnType::DOUBLE: {
                    double parsedDouble = std::stod(ssbuffer.str());
                    memcpy(
                        output.dataBuffer + // Base position 
                        (rowCounter*rowByteSize) + // Row offset
                        (colOffsetBytes), // Column offset
                        &parsedDouble, sizeof(double));
                    std::cout << "Parsed double at [col=" << colCounter << ";row=" 
                    << rowCounter  << "] = "<< parsedDouble << std::endl;
                    colOffsetBytes+=sizeof(double);
                }
                break;
                case ColumnType::FLOAT: {  
                    float parsedFloat = std::stof(ssbuffer.str());
                    memcpy(
                        output.dataBuffer + // Base position 
                        (rowCounter*rowByteSize) + // Row offset
                        (colOffsetBytes), // Column offset
                        &parsedFloat, sizeof(float));
                    std::cout << "Parsed float at [col=" << colCounter << ";row=" 
                    << rowCounter  << "] = "<< parsedFloat << std::endl;
                    colOffsetBytes+=sizeof(float);
                }
                break;
                case ColumnType::STRING: {
                    std::string parsedString = ssbuffer.str();
                    memcpy(
                        output.dataBuffer + // Base position 
                        (rowCounter*rowByteSize) + // Row offset
                        (colOffsetBytes), // Column offset
                        &parsedString, sizeof(std::string));
                    std::cout << "Parsed string at [col=" << colCounter << ";row=" 
                    << rowCounter  << "] = "<< parsedString.c_str() << std::endl;
                    colOffsetBytes+=sizeof(std::string);
                }
                break;
                case ColumnType::BOOL: {
                    std::string content = ssbuffer.str();
                    // Later on apply left and right trimming !!
                    bool parsedBool = content=="True" || content=="true";
                    memcpy(
                        output.dataBuffer + // Base position 
                        (rowCounter*rowByteSize) + // Row offset
                        (colOffsetBytes), // Column offset
                        &parsedBool, sizeof(bool));
                    std::cout << "Parsed bool at [col=" << colCounter << ";row=" 
                    << rowCounter  << "] = "<< parsedBool << std::endl;
                    colOffsetBytes+=sizeof(bool);
                }
                break;
                default:
                    break;
                }

                ssbuffer = std::stringstream();
                colCounter++;
            
            } else {
                ssbuffer << row[c]; // Push char to buffer
            }
        }

        rowCounter++;
    }

    // Close the file handle
    fileHandle.close();

    output.nCols=nCols;
    output.nRows=nRows;
    output.rowByteSize=rowByteSize;
    memcpy(output.columnTypes, colTypes, nColTypes*sizeof(ColumnType));

    return output;
}

bool CSVParser::saveCSV(std::string filePath, DataTable table) {

}


