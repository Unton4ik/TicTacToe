# File: Makefile
# Author: Anton Rieutskyi
# Last Modified: 27/10/2019

# Makefile variables
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Werror -std=c89 -g
EXE = TicTacToe
OBJ = main.o interface.o fileIO.o linkedList.o game.o


# conditional compilation
ifdef Secret
CFLAGS += -D Secret
Secret: clean $(EXE)
endif

ifdef Editor
CFLAGS += -D Editor
Editor: clean $(EXE)
endif


###############
# make rules: #
###############

all: clean $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE)

main.o: main.c game.h fileIO.h interface.h
	$(CC) $(CFLAGS) main.c -c

game.o: game.c game.h bool.h fileIO.h interface.h
	$(CC) $(CFLAGS) game.c -c

interface.o: interface.c interface.h game.h fileIO.h
	$(CC) $(CFLAGS) interface.c -c

fileIO.o: fileIO.c fileIO.h bool.h game.h
	$(CC) $(CFLAGS) fileIO.c -c

linkedList.o: linkedList.c linkedList.h
	$(CC) $(CFLAGS) linkedList.c -c

clean:
	rm -f $(OBJ) $(EXE)
