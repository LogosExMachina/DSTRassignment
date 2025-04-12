# Platform detection
UNAME_S := $(shell uname -s)

# Compiler and linker flags
ifeq ($(UNAME_S), Linux)
    CXX := g++
    LDFLAGS :=
    CXXFLAGS := -Iinclude/
    EXE := csvParserTst
endif

ifeq ($(UNAME_S), Darwin)
    CXX := g++
    LDFLAGS :=
    CXXFLAGS := -Iinclude/
    EXE := csvParserTst
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
    CXX := mingw32-g++
    LDFLAGS := -lmingw32
    CXXFLAGS := -Iinclude/ -std=c++17
    EXE := csvParserTst.exe
endif

# Directories
OBJDIR := obj/
SRCDIR := src/

# Targets
$(EXE): $(addprefix $(OBJDIR), csvParserTst.o csvParser.o dataTable.o strUtils.o)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJDIR)*.o $(EXE)
