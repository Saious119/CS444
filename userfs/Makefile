CC = gcc
COMPILER_WARNINGS = -Wall
GDB_FLAGS = -g 
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
GPROF_FLAGS = -pg -a
LD_LIBS = -lpthread -lreadline -lcurses
CFLAGS = $(COMPILER_WARNINGS) $(GDB_FLAGS)

all: userfs

userfs: userfs.c parse.c crash.c
	$(CC) $(CFLAGS) userfs.c  parse.c crash.c -o userfs $(LD_LIBS)

clean:
	/bin/rm -f userfs *.o *~
