# Platform detection
UNAME_S := $(shell uname -s)

# Compiler and linker flags
ifeq ($(UNAME_S), Linux)
    CXX := g++
    LDFLAGS :=
    CXXFLAGS := -Iinclude/
    EXE1 := csvParserTst
    EXE2 := linkedList
endif

ifeq ($(UNAME_S), Darwin)
    CXX := g++
    LDFLAGS :=
    CXXFLAGS := -Iinclude/
    EXE1 := csvParserTst
    EXE2 := linkedList
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
    CXX := mingw32-g++
    LDFLAGS := -lmingw32
    CXXFLAGS := -Iinclude/ -std=c++17
    EXE1 := csvParserTst.exe
    EXE2 := linkedList.exe
endif

# Directories
OBJDIR := obj/
SRCDIR := src/

# All target
all: $(EXE1) $(EXE2)

# Executable 1
$(EXE1): $(addprefix $(OBJDIR), csvParserTst.o csvParser.o dataTable.o strUtils.o)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

# Executable 2
$(EXE2): $(addprefix $(OBJDIR), linkedListMain.o)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

# Object compilation rule
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJDIR)*.o $(EXE1) $(EXE2)
