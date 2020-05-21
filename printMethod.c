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



/* parse line to integer */	
int parseLineToNum( line ln ) {

	int num = 0;

	num += ln.head.E ;
	num += ln.head.R<<1 ;
	num += ln.head.A<<2 ;
	num += ln.head.funct<<3 ;
	num += ln.head.destReg<<8 ;
	num += ln.head.destAdress<<11 ;
	num += ln.head.srcReg<<13 ;
	num += ln.head.srcAdress<<16;
	num += ln.head.opCode<<18;
	
	return num;
}


/* parse instruction line to integer */
int pirntLine(line * instIamge, int instImageLength) {
	int i;	
	
	for (i=0 ; i < instImageLength ; i++ )  /* prints the instructiom image */
		printf("%06d %06x\n" , 100+i  , parseLineToNum (instIamge[i]) ) ;

	return 1;
}



