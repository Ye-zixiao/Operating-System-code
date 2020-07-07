CC=gcc
OPT=-O -Wall

all:myls.o fsync.o
	$(CC) $(OPT) -o myls myls.o
	$(CC) $(OPT) -o fsync fsync.o

myls.o:myls.c
	$(CC) $(OPT) -c myls.c

fsync.o:fsync.c
	$(CC) $(OPT) -c fsync.c

clean:
	rm myls fsync myls.o fsync.o
