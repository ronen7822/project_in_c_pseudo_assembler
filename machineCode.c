#include "machineCode.h"

static int codeArg(char *arg, int AddMethod, int offset);
int IC;

/* codeInstruction - codes an instruction into the instrcution image
 * INPUT: instrcution opfunct (contains the opcode and funct of the commands), args and their addressing methods
 * OUTPUT: number of words to add to instrctuion counter
 * METHOD:  complete this later on
 */
int buildBinaryCode(int opfunct, char* srcOper, char* destOper, int sourceAdd, int destAdd) {

	int numOfWords = 0;

	/*coding the first word the instrcution it self */
	instIamge[IC].head.opCode = (unsigned int) (opfunct / NUM_CMD); /* gets all the necessary data for the coding */
	instIamge[IC].head.funct = (unsigned int) (opfunct % NUM_CMD);
	instIamge[IC].head.srcAdress = (unsigned int) sourceAdd;
	instIamge[IC].head.destAdress = (unsigned int) destAdd;
	instIamge[IC].head.srcReg = (unsigned int) (sourceAdd == REG) ? (srcOper[1] - '0') : 0;
	instIamge[IC].head.destReg = (unsigned int) (destAdd == REG) ? (destOper[1] - '0') : 0;
	instIamge[IC].head.A = 1; /* in the first word of every instruction A = 1 */
	instIamge[IC].head.E = 0; /* in the first word of every instruction E = 0 */
	instIamge[IC].head.R = 0; /* in the first word of every instruction R = 0 */

	numOfWords += codeArg(srcOper, sourceAdd, 1);
	numOfWords += codeArg(destOper, destAdd, 2);

	return numOfWords; /* number of words added to code image */
}

static int codeArg(char *arg, int AddMethod, int offset) {

	switch (AddMethod) {
	case NON:
	case REG:
		return 0; /* i.e. no word of data added */
	case IMM:
		instIamge[IC + offset].value.data = atoi(arg);
		instIamge[IC + offset].value.labelName = NULL;
		instIamge[IC + offset].value.A = 1;
		instIamge[IC + offset].value.R = 0;
		instIamge[IC + offset].value.E = 0;
		break;
	case DIR:
		instIamge[IC + offset].value.data = 0;
		instIamge[IC + offset].value.labelName = calloc(strlen(arg)+1, sizeof(char));
		strcpy(instIamge[IC + offset].value.labelName, arg);
		instIamge[IC + offset].value.A = 0;
		instIamge[IC + offset].value.R = 1;
		instIamge[IC + offset].value.E = 0;
		break;
	case REL:
		instIamge[IC + offset].value.A = 1;
		instIamge[IC + offset].value.labelName = calloc(strlen(arg)+1, sizeof(char));
		strcpy(instIamge[IC + offset].value.labelName, arg);
		instIamge[IC + offset].value.R = 0;
		instIamge[IC + offset].value.E = 0;
		instIamge[IC + offset].value.data = 0;
		break;
	}

	return 1; /* i.e. a word of data was added */
}
