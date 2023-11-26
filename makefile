CXX = g++
CPPFLAGS =  \
	-Wall \
	-Werror \
	-Wpedantic \
	-pipe \
	-mthreads \
	-O3 \
	-std=gnu++14

OBJDIR = obj
SRCDIR = src
BINDIR = bin
BIN = $(BINDIR)\psa.exe
OBJ = \
	$(OBJDIR)\main.o

.PHONEY: clean all

all: $(OBJ)
	if not exist $(BINDIR) mkdir $(BINDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) -o $(BIN) $(OBJ) -s $(LIBS)

$(OBJDIR)\main.o: $(SRCDIR)\main.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

clean:
	del /s /q $(OBJDIR)
	del /s /q $(BIN)