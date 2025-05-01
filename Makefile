DEBUG ?= 0

# Platform detection
UNAME_S := $(shell uname -s)

# Compiler and linker flags
ifeq ($(UNAME_S), Linux)
    CXX := g++
    LDFLAGS :=
    EXE1 := csvParserTst
    EXE2 := linkedList
endif

ifeq ($(UNAME_S), Darwin)
    CXX := g++
    LDFLAGS :=
    EXE1 := csvParserTst
    EXE2 := linkedList
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
    CXX := mingw32-g++
    LDFLAGS := -lmingw32
    EXE1 := csvParserTst.exe
    EXE2 := linkedList.exe
endif

ifeq ($(DEBUG), 1)
    CXXFLAGS := -Iinclude/ -std=c++17 -g3 -O0
else
    CXXFLAGS := -Iinclude/ -std=c++17 -O2
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
$(EXE2): $(addprefix $(OBJDIR), csvParser.o dataTable.o strUtils.o linkedListMain.o)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

# Object compilation rule
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJDIR)*.o $(EXE1) $(EXE2)
