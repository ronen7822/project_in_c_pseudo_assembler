#include "machineCode.h"

static int codeArg(char *arg, int AddMethod, int IC);

/* codeInstruction - codes an instruction into the instrcution image
 * INPUT: instrcution opfunct (contains the opcode and funct of the commands), args and their addressing methods
 * OUTPUT: number of words to add to instrctuion counter
 * METHOD:  complete this later on
 */
int buildBinaryCode(int opfunct, char* srcOper, char* destOper, int sourceAdd, int destAdd, int IC) {

	int numOfWords = 1;

	/*coding the first word the instrcution it self */
	instIamge[IC].conent.head.opCode = (unsigned int) (opfunct / NUM_CMD); /* gets all the necessary data for the coding */
	instIamge[IC].conent.head.funct = (unsigned int) (opfunct % NUM_CMD);
	instIamge[IC].conent.head.srcAdress = (unsigned int) sourceAdd;
	instIamge[IC].conent.head.destAdress = (unsigned int) destAdd;
	instIamge[IC].conent.head.srcReg = (unsigned int) (sourceAdd == REG) ? (srcOper[1] - '0') : 0;
	instIamge[IC].conent.head.destReg = (unsigned int) (destAdd == REG) ? (destOper[1] - '0') : 0;
	instIamge[IC].conent.head.A = 1; /* in the first word of every instruction A = 1 */
	instIamge[IC].conent.head.E = 0; /* in the first word of every instruction E = 0 */
	instIamge[IC].conent.head.R = 0; /* in the first word of every instruction R = 0 */
	instIamge[IC].conent.head.lineNumber = lineNumber;
	instIamge[IC].type = HEADER_LINE;

	numOfWords += codeArg(srcOper, sourceAdd, IC + 1); /* word next to IC */
	numOfWords += codeArg(destOper, destAdd, IC + 1 + (numOfWords == 2)); /* word next to word next to IC */

	return numOfWords; /* number of words added to code image */
}

static int codeArg(char *arg, int AddMethod, int IC) {

	switch (AddMethod) {
	case NON:
	case REG:
		return 0; /* i.e. no word of data added */
	case IMM:
		instIamge[IC].conent.value.data = atoi(++arg); /* צריך לבדוק שזה מספר!!!  וגם שזה עומד בגדלים הנדרשים*/
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
	if (instIamge[IC].conent.value.A) /* i.e. the addressing method is REL */
		instIamge[IC].conent.value.data = lastCommandIC - symbolValue;
	else /* i.e. addressing method is DIR */
		instIamge[IC].conent.value.data = symbolValue;
}
