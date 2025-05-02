# Platform detection
#UNAME_S := $(shell uname -s)

## Compiler and linker flags
#ifeq ($(UNAME_S), Linux)
#    CXX := g++
#    LDFLAGS :=
#    CXXFLAGS := -Iinclude/
#    EXE1 := csvParserTst
#    EXE2 := linkedList
#endif

#ifeq ($(UNAME_S), Darwin)
#    CXX := g++
#    LDFLAGS :=
#    CXXFLAGS := -Iinclude/
#    EXE1 := csvParserTst
#    EXE2 := linkedList
#endif

#ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
#    CXX := mingw32-g++
#    LDFLAGS := -lmingw32
#    CXXFLAGS := -Iinclude/ -std=c++17
#    EXE1 := csvParserTst.exe
#    EXE2 := linkedList.exe
#endif

# Directories
OBJDIR := obj/
SRCDIR := src/
INCLUDEDIR:= C:\Users\Santi\Development\PROJECTS\BareMetal\y2s2\Assignment\include

CXX:= g++ 
#CXXFLAGS := -Iinclude/ -std=c++17
LDFLAGS:= -lmingw32 

# All target
#all: $(EXE1) $(EXE2)

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

# Executable 1
#$(EXE1): $(addprefix $(OBJDIR), csvParserTst.o csvParser.o dataTable.o strUtils.o)
#	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

# Executable 2
#$(EXE2): $(addprefix $(OBJDIR), linkedListMain.o)
#	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

# Object compilation rule
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -I $(INCLUDEDIR) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJDIR)*.o $(EXE1) $(EXE2)
