#include "csvParser.h"

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

    DataTable table = parser.parseCSV("tstdata.csv", colTypes, nCols);
    
    if(!table.wasInitialized()) {
        std::cout << "Error when loading table from CSV" << std::endl;    
        return 0;
    }

    std::string *colNames = table.getColumnNames();
    
    for(int i=0; i<nCols; i++)
        std::cout << "Col Name [" << i << "] = " << 
        colNames[i].c_str() << std::endl;

    table.print();

    //table.printRowBinary(0);
    //table.printRowBinary(1);
    //table.printRowBinary(2);
    //std::cout << "Value at [col=1;row=2] = " 
    //    << table.getIntAt(1,2) << std::endl;
    
    //table.setIntAt(0,0,10);
    //std::cout << "Value at [col=0;row=0] = " 
    //    << table.getIntAt(0,0) << std::endl;
    
    return 0;
}

