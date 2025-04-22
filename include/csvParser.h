#pragma once

#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <sstream>
#include <fstream>
#include <memory.h>

#include "dataTable.h"
#include "strUtils.h"

class CSVParser {

public: 
    CSVParser();
    ~CSVParser();

    // Loads the contents of a csv file at 'filePath', 
    // returns a DataTable containing its deserialized representation.
    DataTable parseCSV(std::string filePath, ColumnType colTypes[], 
        int stringLengths[], int nColTypes);
    
    // Saves/serializes the contents of 'table' at the 
    // file given by 'filePath'.
    // WARNING: This will overwrite the previous contents
    // of the file
    bool saveCSV(std::string filePath, DataTable table);

private:

};

#endif // CSVPARSER_H