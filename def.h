#ifndef DEF_H
#define DEF_H 0 /* prevent multiple includes */

#define MAX_PROGRAM_SIZE 256 /*max program size line count to be assembled*/
#define MAX_FILE_NAME 1000 /*max file path length for source assembly files*/
#define MAX_LN_LEN 80 /* max line size in assembly source file */
#define MMRY_OFFSET 100  /*memory address of the assembled program after it was loaded to memory*/
#define MAX_LBL_SZ 31  /*max char count for labe name*/
#define MAX_OP_NUM 3 /* maximum args in one line (command name, 1st & 2nd operands */
#define NUM_CMD 16 /* number of commands */
#define ERROR_CODE -1

#define DELETE_SUFFIX(l) fileName[strlen(fileName) - (l)] = '\0' /* delete suffix of file name */

/* struct to store data image */
typedef struct dataNode{
	struct dataNode *next;
	int length;
	unsigned int type: 2; /* use the guide enums to distinguish between .string and .data */
	union { /* intPtr for store numbers (.data), strPtr for store strings (.string) */
		int *intPtr;
		char *strPtr;
	} data;
} dataNode;

extern dataNode *dataImage;

extern int lineNumber; /* extern for error reporting (all functions print the current line number) */

extern int DCF; /* data counter final */
extern int ICF;  /* instruction counter final */

enum guideType {NO_GUIDE, DATA, STR, ENTRY, EXTERN, CODE};/*enum for label types*/
enum instAddressType {NON = -1, IMM = 0, DIR = 1, REL = 2, REG = 3 }; /*enum for operand types*/
enum codingType { ABS = 0, RELOC = 1, EXT = 2 }; /*enum for coding types*/

#endif
