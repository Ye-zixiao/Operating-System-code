all:main.o
	gcc -O -Wall -o main main.c -lpthread
main.o:main.c
	gcc -O -Wall -c main.c -lpthread
clean:
	rm main.o main
