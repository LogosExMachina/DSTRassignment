# Directories
OBJDIR := obj/
SRCDIR := src/
INCLUDEDIR:= C:\Users\Santi\Development\PROJECTS\BareMetal\y2s2\Assignment\include

CXX:= g++
LDFLAGS:= -lmingw32

solution3.exe: $(addprefix $(OBJDIR), solution3.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution2.exe: $(addprefix $(OBJDIR), solution2.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution1.exe: $(addprefix $(OBJDIR), solution1.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

datetst.exe: $(addprefix $(OBJDIR), datetst.o date.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

dynamicArrayTst.exe: $(addprefix $(OBJDIR), dynamicArrayTst.o csvParser.o dataTable.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

csvParserTst.exe: $(addprefix $(OBJDIR), csvParserTst.o csvParser.o dataTable.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

# Object compilation rule
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -I $(INCLUDEDIR) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJDIR)*.o $(EXE1) $(EXE2)
