#include <ctype.h>
#include "machineCode.h"

#define MAX_INT 2097152 /* 2^21 */
#define MIN_INT -2097152 /* -2^21 */

static int codeArg(char *arg, int AddMethod, int IC);
static int codeNumber(int *num, char *arg);

/* codeInstruction - codes an instruction into the instrcution image
 * INPUT: instrcution opfunct (contains the opcode and funct of the commands), args and their addressing methods
 * OUTPUT: number of words to add to instrctuion counter
 * METHOD:  complete this later on
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
		instIamge[IC].conent.value.labelName = calloc(strlen(arg)+1, sizeof(char));
		strcpy(instIamge[IC].conent.value.labelName, arg);
		instIamge[IC].conent.value.A = 0;
		instIamge[IC].conent.value.R = 1;
		instIamge[IC].conent.value.E = 0;
		break;
	case REL:
		instIamge[IC].conent.value.data = 0;
		instIamge[IC].conent.value.labelName = calloc(strlen(arg)+1, sizeof(char));
		strcpy(instIamge[IC].conent.value.labelName, ++arg); /* skip the '&' in relative addressing */
		instIamge[IC].conent.value.A = 1;
		instIamge[IC].conent.value.R = 0;
		instIamge[IC].conent.value.E = 0;
		break;
	}

	instIamge[IC].type = DATA_LINE;
	return 1; /* i.e. a word of data was added */
}

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

int getLineType(int IC) {
	return instIamge[IC].type;
}

char *getSymbol(int IC) {
	return instIamge[IC].conent.value.labelName;
}

int getLineNumber(int IC) {
	return instIamge[IC].conent.head.lineNumber;
}

void setSymbolValue(int IC, int lastCommandIC, int symbolValue) {
	if (instIamge[IC].conent.value.A)  /* i.e. the addressing method is REL */
		instIamge[IC].conent.value.data = (symbolValue - lastCommandIC - MMRY_OFFSET);

	else  /* i.e. addressing method is DIR */
		instIamge[IC].conent.value.data = symbolValue;

}

/* set symbol to be external */
void setExternSymbol(int IC) {
	instIamge[IC].conent.value.E = 1;
	instIamge[IC].conent.value.R = 0;
	instIamge[IC].conent.value.A = 0;
}

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
