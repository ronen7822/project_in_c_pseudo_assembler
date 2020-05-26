#include <ctype.h>
#include "machineCode.h"

#define MAX_INT 2097152 /* 2^21 */
#define MIN_INT -2097152 /* -2^21 */

/* a data structure that contain the fields of header line in instruction image */
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

/* a data structure that contain the fields of data line in instruction image */
typedef struct data {
	int data: 21;
	unsigned int A: 1;
	unsigned int R: 1;
	unsigned int E: 1;
	char *labelName;
} data;

/* a data structure that contains one line in the instruction image */
typedef struct line {
	union { /* content of one line in instruction image */
		cmdHeader head;
		data value;
	} conent;
	unsigned int type : 1; /* type of the line - header or data */
} line;

/* the instruction image */
line instIamge[MAX_PROGRAM_SIZE]; /* MAX_PROGRAM_SIZE is set to be MAX_PROGRAM_SIZE */

/* codeArg - code an argument in the instruction image
 * INPUT: an arg (as a string), its address method and IC (index to locate this)
 * OUTPUT: number of words that located (0 or 1). ERROR (-1) in case of error
 */
static int codeArg(char *arg, int AddMethod, int IC);

/* codeNumber - code number (in immediate addressing) into the instruction image
 * INPUT: an integer pointer, where the number located, and arg (as a sting) to parse
 * OUTPUT: the number is assigned to the pointer. return value is 0 for success and ERROR (-1) for error
 * NOTE: this function make paring - so it should be in parse.c instead of it. but these files are independent intentionally.
 * so a desicion taken to parse the number in for immediate addressing in this file
 */
static int codeNumber(int *num, char *arg);

/* buildBinaryCode - codes an instruction into the instrcution image
 * INPUT: instrcution opfunct (contains the opcode and funct of the commands), args and their addressing methods
 * OUTPUT: number of words to add to instrctuion counter (e.g. return value of 2, means that a command hear and one arg was added
 * return ERROR (-1) of error encountered
 * METHOD: just assign the input to the struct and cal codeArg for assigning the args
 */
int buildBinaryCode(int opfunct, char* srcOper, char* destOper, int sourceAdd, int destAdd, int IC) {

	int numOfWords = 1, tempWordNum, errorFlag = 0;

	/*coding the first word the instrcution it self */
	instIamge[IC].conent.head.opCode = (unsigned int) (opfunct / NUM_CMD); /* gets all the necessary data for the coding */
	instIamge[IC].conent.head.funct = (unsigned int) (opfunct % NUM_CMD);
	instIamge[IC].conent.head.srcAdress = (unsigned int) (sourceAdd != NON) ? sourceAdd : 0;
	instIamge[IC].conent.head.destAdress = (unsigned int) (destAdd != NON) ? destAdd : 0;
	instIamge[IC].conent.head.srcReg = (unsigned int) (sourceAdd == REG) ? (srcOper[1] - '0') : 0;
	instIamge[IC].conent.head.destReg = (unsigned int) (destAdd == REG) ? (destOper[1] - '0') : 0;
	instIamge[IC].conent.head.A = 1; /* in the first word of every instruction A = 1 */
	instIamge[IC].conent.head.E = 0; /* in the first word of every instruction E = 0 */
	instIamge[IC].conent.head.R = 0; /* in the first word of every instruction R = 0 */
	instIamge[IC].conent.head.lineNumber = lineNumber;
	instIamge[IC].type = HEADER_LINE;

	/* code first operand */
	if ((tempWordNum = codeArg(srcOper, sourceAdd, IC + 1)) != ERROR_CODE)
		numOfWords += tempWordNum;
	else
		errorFlag = ERROR_CODE;

	/* code second operand */
	if ((tempWordNum = codeArg(destOper, destAdd, IC + 1 + (numOfWords == 2))) != ERROR_CODE)
		numOfWords += tempWordNum;
	else
		errorFlag = ERROR_CODE;

	return ((errorFlag != ERROR_CODE) ? numOfWords : ERROR_CODE); /* number of words added to code image, or an error */
}

/* codeArg - code an argument in the instruction image
 * INPUT: an arg (as a string), its address method and IC (index to locate this)
 * OUTPUT: number of words that located (0 or 1). ERROR (-1) in case of error
 */
static int codeArg(char *arg, int AddMethod, int IC) {

	int tempNum; /* used for temporary store the parsed number */

	switch (AddMethod) {
	case NON:
	case REG:
		return 0; /* i.e. no word of data added */
	case IMM:
		if (codeNumber(&tempNum, ++arg) < 0) /* skip the # in immediate addressing */
			return ERROR_CODE;
		instIamge[IC].conent.value.data = tempNum;
		instIamge[IC].conent.value.labelName = NULL;
		instIamge[IC].conent.value.A = 1;
		instIamge[IC].conent.value.R = 0;
		instIamge[IC].conent.value.E = 0;
		break;
	case DIR:
		instIamge[IC].conent.value.data = 0;
		instIamge[IC].conent.value.labelName = calloc(strlen(arg) + 1, sizeof(char));
		strcpy(instIamge[IC].conent.value.labelName, arg);
		instIamge[IC].conent.value.A = 0;
		instIamge[IC].conent.value.R = 1;
		instIamge[IC].conent.value.E = 0;
		break;
	case REL:
		instIamge[IC].conent.value.data = 0;
		instIamge[IC].conent.value.labelName = calloc(strlen(arg) + 1, sizeof(char));
		strcpy(instIamge[IC].conent.value.labelName, ++arg); /* skip the '&' in relative addressing */
		instIamge[IC].conent.value.A = 1;
		instIamge[IC].conent.value.R = 0;
		instIamge[IC].conent.value.E = 0;
		break;
	}

	instIamge[IC].type = DATA_LINE;
	return 1; /* i.e. a word of data was added */
}

/* codeNumber - code number (in immediate addressing) into the instruction image
 * INPUT: an integer pointer, where the number located, and arg (as a sting) to parse
 * OUTPUT: the number is assigned to the pointer. return value is 0 for success and ERROR (-1) for error
 * NOTE: this function make paring - so it should be in parse.c instead of it. but these files are independent intentionally.
 * so a desicion taken to parse the number in for immediate addressing in this file
 */
static int codeNumber(int *num, char *arg) {

	int tempNum = 0, errorFlag, sign = 1;
	char *ptr = arg;

	/* check the sign */
	if ((*ptr == '-') || (*ptr == '+')) { /* case of negetive number */
		sign = 44 - *ptr; /* if *ptr is +, sign is 1. if *ptr is -, sign is -1, beacuse + is 43 and - is 45 in ascii */
		ptr++;
	}

	/* parse the number */
	while (isdigit(*ptr)) { /* convert string of number to int */
		tempNum = 10 * tempNum + (*(ptr++) - '0');

		if ((tempNum > MAX_INT) || (tempNum < MIN_INT)) { /* check every iteration to prevent overflow */
			printf("error in %d: a number does not fit the memory, should be from %d to %d\n", lineNumber, MIN_INT, MAX_INT);
			errorFlag = ERROR_CODE;
		}
	}

	/* when encounter a char that is not a number, check if it the end of the arg - if not, it is an error */
	if (*ptr != '\0') {
		printf("error in %d: invalid character (%c) in number - not a digit\n", lineNumber, *ptr);
		errorFlag = ERROR_CODE;
	}

	if (errorFlag == ERROR_CODE)
		return ERROR_CODE;

	*num = tempNum * sign; /* set number into the input variable */
	return 0;
}

/* getLineType - return if a line is a header or data (code an argument)
 * INPUT: an index in instruction image
 * OUTPUT: the type of the line in this index
 */
int getLineType(int IC) {
	return instIamge[IC].type;
}

/* getSymbol - for data lines, if the data is a symbol to resolve, the symbol is saved in the line index during the first scan.
 * this function return the symbol for a line (if symbol is NULL, there is no symbol and second scan skip this line)
 * INPUT: an index in instruction image
 * OUTPUT: symbol in this line
 */
char *getSymbol(int IC) {
	return instIamge[IC].conent.value.labelName;
}

/* getLineNumber - for header lines, this function return the number of the line in the original file (".as" file) for error prints
 * INPUT: an index in instruction image
 * OUTPUT: the line number in the original file that corresponds to this index in instruction image
 */
int getLineNumber(int IC) {
	return instIamge[IC].conent.head.lineNumber;
}

/* setSymbolValue - assign the value of symbol (during second scan)
 * INPUT: an index in instruction image (IC), index of the command in instruction image, value of a symbol
 * OUTPUT: void, but assign the value of a the symbol in the instruction image
 * METHOD: for DIR method, assign the value of the symbol, for REL method, assign the difference between header IC (lastCommandIC) and symbol value
 */
void setSymbolValue(int IC, int lastCommandIC, int symbolValue) {
	if (instIamge[IC].conent.value.A)  /* i.e. the addressing method is REL */
		instIamge[IC].conent.value.data = (symbolValue - lastCommandIC - MMRY_OFFSET);

	else  /* i.e. addressing method is DIR */
		instIamge[IC].conent.value.data = symbolValue;

}

/* setExternSymbol - for symbol that detected to be external, set the refrence bits to represent that this symbol is external
 * INPUT: an index in instruction image to set to be external
 * OUTPUT: void, but reference bits in the line marked as external
 */
void setExternSymbol(int IC) {
	instIamge[IC].conent.value.E = 1;
	instIamge[IC].conent.value.R = 0;
	instIamge[IC].conent.value.A = 0;
}

/* lineForPrint - calculate the value of a line, to print it in hex in the output file
 * INPUT: an index in instruction image
 * OUPUT: a numeric value of this line
 */
int lineForPrint(int IC) {
		int ret = 0; /* this number is the return value */

		if (instIamge[IC].type == HEADER_LINE) {
			ret += instIamge[IC].conent.head.E ;
			ret += instIamge[IC].conent.head.R <<1;
			ret += instIamge[IC].conent.head.A <<2;
			ret += instIamge[IC].conent.head.funct <<3;
			ret += instIamge[IC].conent.head.destReg << 8;
			ret += instIamge[IC].conent.head.destAdress << 11;
			ret += instIamge[IC].conent.head.srcReg << 13;
			ret += instIamge[IC].conent.head.srcAdress << 16;
			ret += instIamge[IC].conent.head.opCode << 18;
		}

		/* case of data line */
		else {
			ret += instIamge[IC].conent.value.E;
			ret += instIamge[IC].conent.value.R << 1;
			ret += instIamge[IC].conent.value.A << 2;
			ret += instIamge[IC].conent.value.data << 3;
		}

		return ret;
}
