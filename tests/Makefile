IDIR =/usr/include/getopt++
CC=g++
CFLAGS=-I$(IDIR) -std=c++11 -fprofile-arcs -ftest-coverage
VPATH=../src

ODIR=./obj
LDI =../lib

LIBS=-lcriterion

_DEPS = misc.h command_impl.h parser_impl.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = command.o parser.o gopp_tests.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -c -o $@ 
gopp_tests: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
