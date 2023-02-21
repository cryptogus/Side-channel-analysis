CC = gcc

CFLAGS	:= -Wall -Wextra -g

LFLAGS =-lm

all: Align

Align: alignment.o
	$(CC) $(CFLAGS) alignment.h -o Align alignment.o $(LFLAGS)

alignment.o: alignment.c

clean:
		rm -f *.o
		rm -f Align