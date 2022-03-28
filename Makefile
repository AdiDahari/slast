all: slast

slast: slast.c
	gcc -Wall slast.c -o slast

clean:
	rm -rf slast.o slast