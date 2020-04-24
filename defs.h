/* defs.h - contains main definitions, structures for the assembler. More specificaly:
 * constants
 *
 *
 */

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
const char *guidanceTypes[]={".data",".string",".mat",".extern",".entry",NULL};

/*Char array to hold the instruction names */
const char *instructionNames[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop",NULL};

typedef struct instInfo {
	char *instName; /* insturction name */
	int opCode; /* insturction opCode */
	int funct; /* insturction funct, 0 if no funct */
	int firstOp; /* valid addressing methods for first arg */
	int secondOp; /* valid addressing methods for first arg */
};

typedef struct psw_register {
	unsinged int zero_flag:1;
};

int lineNumber;
int dataImage[MAX_PROGRAM_SIZE];
int instructionImage[MAX_PROGRAM_SIZE];
int IC; /* instruction counter */
int DC; /* data counter  */

enum { IMM = 0, DIR = 1, REL = 2, REG = 3 }; /*enum for operand types*/
enum { ABS = 0, RELOC = 1, EXT = 2 }; /*enum for coding types*/

typedef struct operandNode * optr; /*pointer to operandNode object*/

/*operandNode data type. includes operMemAddress field for IC counter value
(int) and next field to point to next object in linked list*/
typedef struct operandNode
{
   int operMemAddress;
   optr next;
}
operandNode;

/*label data type. includes vars for label name, label type, if label is used as entry, address in memory,pointer to operandNode list which contains all operands' IC values that are using this label (in use only for external labels)*/
typedef struct label
{
	char name[MAX_LBL_SZ+1]; /* because there is '/0' in the end */
	int isEntry;
	int counterValue;
	optr operatorNodePtr;
}
label;

typedef struct labelNode * lptr; /*pointer to labelNode object*/

/*labelNode data type. includes label object and a pointer to the next labelNode object in list*/
typedef struct labelNode
{
   label label;
   lptr next;
}
labelNode;

