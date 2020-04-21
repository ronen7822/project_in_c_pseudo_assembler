FLAGS_G = -g -ansi -Wall -pedantic
FLAGS_C = -c -ansi -Wall -pedantic

all: legalTable

symbolTable.o: symbolTable.c defs.h
	gcc $(FLAGS_C) symbolTable.c -o symbolTable.o

legalTable.o: legalTable.c defs.h
	 gcc $(FLAGS_C) -lm legalTable.c -o legalTable.o

clean:
	rm *.o numbers
