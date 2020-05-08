#include "symbolTable.c"
#include "def.h"


/* extracrts the opCode according to the instruction name */
static int extractOpCode (char* commandName );

/* extracrts a number from the string */
static int extractNum (char *str);

/* extracrts the funct according to the instruction name */
static int extractFunct (char* commandName );

/* checks if the string is a register name */
static int isRegister(char *str);

/* should be fined in  symbolTable.c */
int isExternal(char *label); 

/* should be fined in  symbolTable.c */
int findMemAddress( char * label); 
