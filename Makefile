DEBUG ?= 0

# Platform detection
#UNAME_S := $(shell uname -s)

# Compiler and linker flags
# ifeq ($(UNAME_S), Linux)
#     CXX := g++
#     LDFLAGS :=
#     EXE1 := csvParserTst
#     EXE2 := linkedList
# endif
#
# ifeq ($(UNAME_S), Darwin)
#     CXX := g++
#     LDFLAGS :=
#     EXE1 := csvParserTst
#     EXE2 := linkedList
# endif
#
# ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
#     CXX := mingw32-g++
#     LDFLAGS := -lmingw32
#     EXE1 := csvParserTst.exe
#     EXE2 := linkedList.exe
# endif
#
# ifeq ($(DEBUG), 1)
#     CXXFLAGS := -Iinclude/ -std=c++17 -g3 -O0
# else
#     CXXFLAGS := -Iinclude/ -std=c++17 -O2
# endif

# Directories
OBJDIR := obj/
SRCDIR := src/
INCLUDEDIR:= C:\Users\Santi\Development\PROJECTS\BareMetal\y2s2\Assignment\include

CXX:= g++
#CXXFLAGS := -std=c++17
LDFLAGS:= -lmingw32

# All target
#all: $(EXE1) $(EXE2)

solutionArray_jaden.exe: $(addprefix $(OBJDIR), solutionArray_jaden.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

wenyisolution.exe: $(addprefix $(OBJDIR), linkedListMain.o csvParser.o dataTable.o strUtils.o)
	$(CXX) -std=c++17 -g $^ -o $@ $(LDFLAGS)

solution3Array_sara.exe: $(addprefix $(OBJDIR), solution3Array_sara.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution2Array_sara.exe: $(addprefix $(OBJDIR), solution2Array_sara.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution1Array_sara.exe: $(addprefix $(OBJDIR), solution1Array_sara.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution3Array_santiago.exe: $(addprefix $(OBJDIR), solution3Array_santiago.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution2Array_santiago.exe: $(addprefix $(OBJDIR), solution2Array_santiago.o csvParser.o dataTable.o date.o strUtils.o)
	$(CXX) -g $^ -o $@ $(LDFLAGS)

solution1Array_santiago.exe: $(addprefix $(OBJDIR), solution1Array_santiago.o csvParser.o dataTable.o date.o strUtils.o)
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
# $(EXE2): $(addprefix $(OBJDIR), csvParser.o dataTable.o strUtils.o linkedListMain.o)
# 	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

# Object compilation rule
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -I $(INCLUDEDIR) -c $^ -o $@ 
#-std=c++17

# Clean
clean:
	rm -f $(OBJDIR)*.o $(EXE1) $(EXE2)
