all: AES

AES: AES128.o main.o
		gcc AES128.o main.o -o AES

main.o: main.c AES128.h

AES128.o: AES128.c

clean:
		rm -f *.o
		rm -f AES