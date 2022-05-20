VPATH = SRC INCLUDE 
IDIR=../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=make_exercise
LDIR=../lib

LIBS=-lm

_DEPS=functions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o hello.o factorial.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

%(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# .PHONY: clean

# clean:
# 	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 