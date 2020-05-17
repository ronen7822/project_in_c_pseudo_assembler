#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "def.h"

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
} cmdHeader;


typedef struct data {
	char *labelName;
	int data: 21;
	unsigned int A: 1;
	unsigned int R: 1;
	unsigned int E: 1;
} data;

typedef union { /* instruction line */
	cmdHeader head;
	data value;
} line;

line instIamge[MAX_PROGRAM_SIZE]; /* MAX_PROGRAM_SIZE is set to be MAX_PROGRAM_SIZE */
/* line dataImage[MAX_PROGRAM_SIZE]; */

int buildBinaryCode(int opfunct, char* srcOper, char* destOper, int sourceAdd, int destAdd);
