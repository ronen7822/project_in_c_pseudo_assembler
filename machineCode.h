#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "def.h"

#define HEADER_LINE 0
#define DATA_LINE 1

typedef struct cmdHeader {
	unsigned int opCode: 6; /* insturction opCode */
	unsigned int srcAdress: 2 ; /*  addressing method for destation register */
	unsigned int srcReg: 3 ; /* source register */
	unsigned int destAdress: 2 ; /* addressing method for source register */
	unsigned int destReg: 3 ; /* destation register */
	unsigned int funct: 5 ; /* insturction funct, 0 if no funct */
	unsigned int A: 1; /* abslute Linkange */
	unsigned int R: 1; /* Relocatable Linkange */
	unsigned int E: 1; /* external Linkange */
	unsigned int lineNumber; /* contain the line of the command */
} cmdHeader;


typedef struct data {
	int data: 21;
	unsigned int A: 1;
	unsigned int R: 1;
	unsigned int E: 1;
	char *labelName;
} data;



typedef struct line {
	union { /* content of one line in instruction image */
		cmdHeader head;
		data value;
	} conent;
	unsigned int type : 1; /* type of the line - header or data */
} line;

line instIamge[MAX_PROGRAM_SIZE]; /* MAX_PROGRAM_SIZE is set to be MAX_PROGRAM_SIZE */

int buildBinaryCode(int opfunct, char* srcOper, char* destOper, int sourceAdd, int destAdd, int IC);

char *getSymbol(int IC);

int getLineType(int IC);

char *getSymbol(int IC);

void setSymbolValue(int IC, int lastCommandIC, int symbolValue);

void setExternSymbol(int IC);

int getLineNumber(int IC);

int lineForPrint(int IC);

