
#include <stdlib.h>
#define MAX_PROGRAM_SIZE 256 /*max program size line count to be assembled*/
#define ASM_SOURCE_SUFF ".as"	/*assembly source file suffix*/
#define ASM_OBJECT_SUFF ".ob"	 /*assembly object file suffix*/
#define ASM_ENTRIES_SUFF ".ent"  /*assembly entries file suffix*/
#define ASM_EXTERNALS_SUFF ".ext"  /*assembly externals file suffix*/
#define MAX_FILE_NAME 1000 /*max file path length for source assembly files*/
#define MAX_LN_LEN 80 /* max line size in assembly source file */
#define MMRY_OFFSET 100  /*memory address of the assembled program after it was loaded to memory*/
#define MAX_LBL_SZ 31  /*max char count for labe name*/


/*Char array to hold the guidance types */
const char *guidanceTypes[]={".data",".string",".extern",".code",".entry",NULL};

/*Char array to hold the instruction names */
const char *instructionNames[]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop",NULL};


typedef struct instInfo {
	char *instName; /* insturction name */
	int opCode; /* insturction opCode */
	int funct; /* insturction funct, 0 if no funct */
	int firstOp; /* valid addressing methods for first arg */
	int secondOp; /* valid addressing methods for first arg */
};

typedef struct psw_register 
{
	unsinged int zero_flag:1;
};

extern int lineNumber;
extern int dataImage[MAX_PROGRAM_SIZE];
extern int instructionImage[MAX_PROGRAM_SIZE];
extern int IC; /* instruction counter */
extern int DC; /* data counter  */

typedef enum labelType {DATA,CODE,EXTERNAL} labelType;/*enum for label types*/
typedef enum instAddressType {NON =-1, IMM = 0, DIR = 1, REL = 2, REG = 3 }; /*enum for operand types*/
typedef enum codingType { ABS = 0, RELOC = 1, EXT = 2 }; /*enum for coding types*/


