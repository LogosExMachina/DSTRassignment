
# Replace this by your include folder...
INCLUDE:=C:/Users/Santi/Development/PROJECTS/BareMetal/y2s2/Assignment/include/
OBJDIR:=obj/
SRCDIR:=src/

csvParserTst.exe: $(addprefix $(OBJDIR), csvParserTst.o csvParser.o dataTable.o strUtils.o)
	g++ -I $(INCLUDE) -g $^ -o $@ -lmingw32

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	g++ -I $(INCLUDE) -c $^ -o $@