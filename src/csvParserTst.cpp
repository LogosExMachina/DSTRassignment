#include "csvParser.h"
#include "strUtils.h"

int main(int argc, char** argv) {

    CSVParser parser;
    ColumnType colTypes[] = {
        ColumnType::INT, 
        ColumnType::DOUBLE, 
        ColumnType::FLOAT,
        ColumnType::STRING,
        ColumnType::BOOL
    };
    int nCols = sizeof(colTypes)/sizeof(ColumnType);

    int stringLengths[] = {
        -1,
        -1,
        -1,
        16,
        -1
    };

    DataTable table = parser.parseCSV("data/tstdata.csv", colTypes, stringLengths, nCols);
    
    if(!table.wasInitialized()) {
        std::cout << "Error when loading table from CSV" << std::endl;    
        return 0;
    }

    std::string *colNames = table.getColumnNames();
    
    for(int i=0; i<nCols; i++)
        std::cout << "Col Name [" << i << "] = " << 
        colNames[i].c_str() << std::endl;

    table.print();
    
    for(int i=0; i<table.getnRows(); i++) {
        table.printRowBinary(i);
    }


    bool saveSuccess = parser.saveCSV(std::string("data/newdata.csv"), table);
    std::cout << (saveSuccess?
    "> Data successfully serialized" :
    "> Error when serializing data...") 
    << std::endl;


    return 0;
}

