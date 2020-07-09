CC=gcc
OPT=-O -Wall

all:myls.o fsync.o mystat.o mytail.o
	$(CC) $(OPT) -o myls myls.o
	$(CC) $(OPT) -o fsync fsync.o
	$(CC) $(OPT) -o mystat mystat.o
	$(CC) $(OPT) -o mytail mytail.o

myls.o:myls.c
	$(CC) $(OPT) -c myls.c

fsync.o:fsync.c
	$(CC) $(OPT) -c fsync.c

mystat.o:mystat.c
	$(CC) $(OPT) -c mystat.c

mytail.o:mytail.c
	$(CC) $(OPT) -c mytail.c

clean:
	rm myls fsync mystat mytail
	rm mytail.o myls.o fsync.o mystat.o
