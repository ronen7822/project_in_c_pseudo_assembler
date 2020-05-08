#include "symbolTable.c"
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
	int data: 21;
	unsigned int A: 1;
	unsigned int R: 1;
	unsigned int E: 1;
} data;

typedef union { /* instruction line */
	cmdHeader head;
	data value;
} line;

typedef struct {
	line codeLine;
	int IC;
	int L;
} linePlus;

line instIamge[MAX_PROGRAM_SIZE]; /* MAX_PROGRAM_SIZE is set to 256 */
/* line dataImage[MAX_PROGRAM_SIZE]; */
/* e.g : mov, r1, #4, address(r1), address(4) */


/* extracrts the opCode according to the instruction name */
static int extractOpCode (char* commandName );

/* extracrts a number from the string */
static int extractNum (char *str);

/* extracrts the funct according to the instruction name */
static int extractFunct (char* commandName );

/* checks if the string is a register name */
static int isRegister(char *str);

/* should be dfined in  symbolTable.c */
int isExternal(char *label); 

/* should be dfined in  symbolTable.c */
int findMemAddress( char * label); 
