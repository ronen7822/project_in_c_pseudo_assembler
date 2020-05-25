FLAGS_G = -g -ansi -Wall -pedantic
FLAGS_C = -c -ansi -Wall -pedantic
ALL_ASSIST_FILES = parse.c parse.h machineCode.c machineCode.h legalTable.c legalTable.h symbolTable.c symbolTable.h def.h
ALL_OFILES = main.o firstScan.o secondScan.o parse.o machineCode.o legalTable.o symbolTable.o buildFiles.o

all: main

filesBuilder.o: filesBuilder.c def.h
	gcc $(FLAGS_C) filesBuilder.c -o  filesBuilder.o

symbolTable.o: symbolTable.c def.h
	gcc $(FLAGS_C) symbolTable.c -o symbolTable.o

legalTable.o: legalTable.c def.h
	 gcc $(FLAGS_C) -lm legalTable.c -o legalTable.o 

machineCode.o: machineCode.c machineCode.h def.h
	gcc $(FLAGS_C) machineCode.c -o machineCode.o

parse.o: parse.c parse.h def.h
	gcc $(FLAGS_C) parse.c -o parse.o
	
firstScan.o: $(ALL_ASSIST_FILES) firstScan.c firstScan.h
	gcc $(FLAGS_C) firstScan.c -o firstScan.o
	
secondScan.o: $(ALL_ASSIST_FILES) secondScan.c secondScan.h
	gcc $(FLAGS_C) secondScan.c -o secondScan.o
	
buildFiles.o: $(ALL_ASSIST_FILES) buildFiles.c buildFiles.h
	gcc $(FLAGS_C) buildFiles.c -o buildFiles.o
	
main.o: firstScan.c firstScan.h secondScan.c secondScan.h main.c
	gcc $(FLAGS_C) main.c -o main.o
	
main: $(ALL_OFILES)
	gcc $(FLAG_G) $(ALL_OFILES) -lm -o main

clean:
	rm *.o main
