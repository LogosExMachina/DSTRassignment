#pragma once

#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <sstream>
#include <fstream>
#include <memory.h>
#include "dataTable.h"

class CSVParser {

public: 
    CSVParser();
    ~CSVParser();

    DataTable parseCSV(std::string filePath, ColumnType colTypes[], int nColTypes);
    bool saveCSV(std::string filePath, DataTable table);

private:

};

#endif // CSVPARSER_H