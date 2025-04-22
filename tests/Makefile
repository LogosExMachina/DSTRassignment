
# Replace this by your project folder...
PROJECTDIR:=C:/Users/Santi/Development/PROJECTS/BareMetal/y2s2/Assignment/

INCLUDE:=$(PROJECTDIR)include/
OBJDIR:=$(PROJECTDIR)obj/
SRCDIR:=$(PROJECTDIR)src/

csvParserTst.exe: $(addprefix $(OBJDIR), csvParser.o dataTable.o strUtils.o) csvParserTst.o
	g++ -I $(INCLUDE) -g $^ -o $@ -lmingw32

csvParserTst.o: csvParserTst.cpp
	g++ -I $(INCLUDE) -c $^ -o $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	g++ -I $(INCLUDE) -c $^ -o $@