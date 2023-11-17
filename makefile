# *  Define the name of the makefile.                                        *

MAKNAM = makefile

# *  Define the directories in which to search for library files.            *

LIBDRS =

# *  Define the directories in which to search for include files.            *

INCDRS =

# *  Define the library files.                                               *

LIBFLS =

# *  Define the source files.                                                *

SRCFLS = main.c\
				 board.h\
				 creature.h\
				 list.h

# *  Define the object files.                                                *

OBJFLS = main.o\
				 board.o\
				 creature.o\
				 list.o

# *  Define the executable.                                                  *

EXE    = ecosystem

# *  Define the compile and link options.                                    *

CC     = gcc
LL     = gcc
CFLAGS = -Wall -g

# *  Define the rules.                                                       *

$(EXE): $(OBJFLS)
	$(LL) $(LFLAGS) -o $@ $(OBJFLS) $(LIBDRS) $(LIBFLS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $(INCDRS) $<

all:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) $(EXE)

clean:
	-rm $(EXE)
	-rm $(OBJFLS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

main.o: board.h board.c creature.h creature.c
board.o: board.h creature.h list.h
creature.o: creature.h
list.o: list.h

