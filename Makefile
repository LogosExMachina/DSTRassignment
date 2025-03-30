
csvParserTst.exe: csvParserTst.o csvParser.o dataTable.o
	g++ $^ -o $@ -lmingw32

%.o: %.cpp
	g++ -c $^ -o $@