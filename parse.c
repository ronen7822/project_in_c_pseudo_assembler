#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "def.h"

#define MAX_GUIDE_LEN 6 /* maximum length of guidance ("string" is the longest) */
#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX_INT 2097152 /* 2^21 */
#define MIN_INT -2097152 /* -2^21 */
#define MAX_CHAR 126 /* MAX, MIN are range of visible characters */
#define MIN_CHAR 32
#define isVaidChar(c) ((c < MAX_CHAR) && (c > MIN_CHAR))

static int addArg(char* cmd, char *argv[MAX_OP_NUM], int i, int argStart, int argc);
static void flushArgv(char *argv[MAX_OP_NUM]);
int lineNumber;

/* getLabel - extract a label from a line
 * INPUT: a line
 * OUTPUT: a string (char*) of the label, or NULL if there is no labal in the line
 * METHOD: return the part in the line from the first non-white character space until ':'
 */
char *getLabel(char *line) {
	char *label = calloc(MAX_LN_LEN + 1, sizeof(char));
	char *startLabel = line, *endLabel;

	if ((endLabel = strchr(line, ':')) == NULL) /* no label in line */
		return NULL;

	while ((*startLabel == ' ') || (*startLabel == '\t'))
		++startLabel;

	strncpy(label, startLabel, endLabel - startLabel); /* copy label */
	label[endLabel - startLabel] = '\0';
	return label;
}

char *getAnotherLabel(char *line) {
	char *label = calloc(MAX_LN_LEN + 1, sizeof(char));
	char *startLabel, *endLabel;

	while ((*startLabel == ' ') || (*startLabel == '\t'))
		++startLabel;

	endLabel = startLabel;
	while (!isspace(*endLabel))
		++endLabel;

	strncpy(label, startLabel, endLabel - startLabel); /* copy label */
	label[endLabel - startLabel] = '\0';
	return label;
}


/* labelValid - check validity of label
 * INPUT: a label
 * OUTPUT: validity of the label (start with a letter, contains only nums and letters, in a proper length)
 */
int labelValid(char *label) {
	int validity = 0;

	/* check if the label is in a proper length */
	if (strlen(label) > MAX_LBL_SZ) {
		printf("error in %d: label is too long, maximum length is %d", lineNumber, MAX_LBL_SZ);
		validity = -1;
	}

	/* check if label starts with a letter */
	if (!isalpha(*label)) {
		printf("error in %d:: label should start with a letter", lineNumber);
		validity = -1;
	}

	/* check if label contains only numbers and letters */
	while (*(++label))
		if (!isalnum(*label)) {
			printf("error in %d:: label should contain only letters and numbers", lineNumber);
			validity = -1;
		}

	return validity;
}

int getGuideType(char* linePtr) {
	char *guide = calloc(MAX_GUIDE_LEN +1 , sizeof(char));
	int length = 0; /* length of guide, if exist */

	while ((*linePtr == ' ') || (*linePtr == '\t'))
		++linePtr;

	if (*(linePtr++) != '.')
		return NO_GUIDE; /* no guidance is this line */

	/* calculate length of guidance statement */
	while (!isspace(linePtr + length))
		length += sizeof(char);

	strncpy(guide, linePtr, MIN(length, MAX_GUIDE_LEN));

	if (strcmp(guide, "data"))
		return DATA;
	else if (strcmp(guide, "string"))
		return STR;
	else if (strcmp(guide, "entry"))
		return ENTRY;
	else if (strcmp(guide, "extern"))
		return EXTERN;

	printf("error in %d: guid name is not valid", lineNumber);
	return -1; /* error code */
}


/* getAddMthd - amnalize the operand to get its addressing method
 * INPUT: an operand (as string)
 * OUTPUT: indication of its addressing method (NON = no operand)
 */
int getAddMthd(char* op) {

	if (!strlen(op)) /* no operand */
		return NON;

	else if (op[0] == '#')
		return IMM;

	else if (op[0] == '&')
		return REL;

	else if ((op[0] = 'r') && (op[1] >= '0') && (op[1] <= '7') && (strlen(op) == 2))
		return REG;

	/* there is no specific character for DIR. if the label is not valid, error will be detected in second scan */
	return DIR;
}


/* getNumbers - extracts the numbers into array of chars inside dataContent
 * INPUT: a line and dataContent union
 * OUTPUT: integer. -1 in case of an eror, 0 otherwise
 */
dataNode *getNumbers(char* line) {

	dataNode *node = NULL;
	long int tempNum; /* long for big numbers */
	int i = 0, errorFlag = 0;
	char *ptr;
	node->length = 1;
	node->next = NULL;

	for (ptr = line; ptr != '\0'; ptr++)
		node->length += (*ptr == ',');

	node->data.intPtr = calloc(node->length, sizeof(int));

	ptr = line;
	while (i < node->length) {

		while (isspace(*(ptr++))); /* skip white spaces */

		tempNum = 0;
		while (isdigit(*ptr)) /* convert string to int */
			tempNum = 10 * tempNum + (*(ptr++) - '0');

		if ((tempNum > MAX_INT) || (tempNum < MIN_INT)) {
			printf("error in %d: a number (%ld) does not fit the memory\n", lineNumber, tempNum);
			errorFlag = 1;
		}


		node->data.intPtr[i++] = (int) tempNum;

		if (!isdigit(*ptr) && !isspace(*ptr) && !(*ptr == ',')) {
			printf("error in %d: invalid character\n", lineNumber);
			errorFlag = 1;
			while (!isspace(*ptr) && !(*(ptr++) == ',')); /* skip until next space or comma for prevent multiple errors*/
		}

		while (isspace(*ptr)) /* skip white spaces */
			ptr++;

		if ((i != node->length - 1) && (*(ptr++) != ',')) {
			printf("error in %d: comma expected but not found\n", lineNumber);
			errorFlag = 1;
		}
	}

	/* check if there are more arguments than commas */
	while ((*ptr != '\0') && (*ptr != '\n') && (*ptr != EOF)) {
		if (!isspace(*(ptr++))) {
			printf("error in %d: arguments should be seperated by comma\n", lineNumber);
			errorFlag = -1;
		}
	}

	if (errorFlag) /* free memory allocation */
		free(node->data.intPtr);


	/* if node.data is NULL, an error detected. otherwise, succeed */
	return node;
}

/* getStirng - extracts string into array of chars inside dataNode
 * INPUT: a line and dataContent union
 * OUTPUT: integer. -1 in case of an eror, 0 otherwise
 */
dataNode *getString(char* line) {

	dataNode *node =NULL;
	int i = 0, errorFlag = 0;
	char *strStart, *strEnd;
	node->next = NULL;

	/* case of no quotation mark in line at all */
	if ((strStart = strchr(line, '\"')) == NULL) {
		printf("error in %d: string should start with quotation mark", lineNumber);
		errorFlag = 1;
	}

	/* case of string no quotation mark in the end of the string */
	if ((strEnd = strchr(++strStart, '\"')) == NULL) {
		printf("error in %d: string should end with quotation marks", lineNumber);
		errorFlag = 1;
	}

	node->data.strPtr = calloc((strEnd - strStart + 1), sizeof(char)); /* stringPtr points to the first char in the string */


	/* '\"' represent end of string */
	while ((node->data.strPtr[i] = strStart[i]) != '\"') {
		if (!isVaidChar(*strStart)) { /* invalid char in string */
			printf("error in %d: invalid char in string", lineNumber);
			errorFlag = 1;
		}
		i++;
	}
	node->data.strPtr[i] = '\0';

	/* check for no extraneous text end of the string */
	while (++strEnd != '\0') {
		if (!isspace(*strEnd)) { /* here, strEnd doesn't points to the string end, but to the rest chars in the line */
			printf("error in %d: extraneous text after end of string", lineNumber);
			errorFlag = 1;
		}
	}

	if (errorFlag)
		free(node->data.strPtr);

	return node;
}

/* parseCmd - take command and extract the args
 * parseCmd check the command syntax (commas, no more than 4 args, no invalid chars, etc.)
 * but NOT the command context (correct command to run, number of args correlate to command, types of args etc.)
 * INPUT: a command (cmd) and array to store the arguments
 * OUTPUT: argument count (argc), -1 for invalid syntax. the arguments in argv
 */
int parseCommand(char *argv[MAX_OP_NUM], char *cmd) {

	enum inWord {OUTWORD, INWORD} iw = OUTWORD; /* mark end of arg */
	enum isComma {FORBID_COMMA, NEED_COMMA, WAS_COMMA} ic = FORBID_COMMA; /* enforce right use in commas */
	int i, argStart, argc = 0, errorFlag = 0;

	flushArgv(argv); /* flush (reset, clean) argv before another use */

	for (i = 0, argStart = 0; cmd[i] || (argc == MAX_OP_NUM ) ; ++i) {
		/* CASE: white space. DO: enter word into argv[argc++] */
		if (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n' || cmd[i] == EOF) {
			if (iw == INWORD) {
				if ((argc = addArg (cmd, argv, i, argStart, argc)) < 0)
					errorFlag = 1;
				ic = argc > 1 ? NEED_COMMA : FORBID_COMMA;
				iw = OUTWORD;
			}
			/* CASE: end of line */
			if (cmd[i] == '\n' || cmd[i] == EOF || cmd[i] == '\0')
				break;
		}

		/* CASE: no white space or EOL and argc = 3: extraneous text after end of command */
		else if (argc >= MAX_OP_NUM ) {
			printf("error in %d: extraneous text after end of command\n", lineNumber);
			return -1; /* there is no need to continue */
		}

		/* CASE: comma. DO: same as white space but check for no double commas */
		else if (cmd[i] == ',') {
			if (iw == INWORD) { /* case of end of word */

				if ( (argc = addArg(cmd, argv, i, argStart, argc)) < 0 )
					errorFlag = 1; /* error code. error message printed in addArg */
				if (argc == MAX_OP_NUM ){
					printf("error in %d: extraneous text after end of command\n", lineNumber);
					errorFlag = -1;
				}
				ic = argc > 1 ? NEED_COMMA : FORBID_COMMA;
				iw = OUTWORD;
			}

			if (ic == FORBID_COMMA){ /* error: case of wrong comma */
				printf("error in %d: illegal comma before first operand\n", lineNumber);
				ic = WAS_COMMA;
				errorFlag = -1;
			}
			else if (ic == WAS_COMMA){ /* error: case of double commas */
				printf("error in %d: multiple consecutive commas\n", lineNumber);
				errorFlag = -1;
			}
			else if (ic == NEED_COMMA)
				ic = WAS_COMMA;

		}

		/* CASE: part of operand */
		else {
			if (ic == NEED_COMMA) {
				printf("error in %d: missing comma between operands\n", lineNumber);
				errorFlag = -1; /* error code */
			}
			if (iw == OUTWORD) {
				iw = INWORD;
				argStart = i; /* the first char of the next arg */
			}
		}
	}

	return errorFlag;
}


/* addArg - subfunction of parseCmd. Called to add argument into argv
 * if failed, return -1 if fail (too long arg), update arg count (argc) if succeed
 * INPUT: a command, indexes of the arg (argStart -> i), argv, argc
 * OUTPUT: argc value (-1 if fail)
 */
static int addArg(char* cmd, char *argv[MAX_OP_NUM ], int i, int argStart, int argc) {

	if (i - argStart > MAX_LN_LEN)
		printf("error in %d: arg %d is too long, maximum length is %d\n",lineNumber, argc, MAX_LN_LEN);
	else {
		strncpy(argv[argc], &cmd[argStart], i - argStart);
		argv[argc++][i - argStart] = '\0';
	}

	return argc;
}

/* flushArgv - flush argv for another use
 * INPUT: argv
 * OUTPUT: clean argv (all elements are empty strings
 */
static void flushArgv(char *argv[MAX_OP_NUM]) {

	int i;
	for (i = 0; i < MAX_OP_NUM; i++)
		*argv[i] = '\0';
}

void printArgv(char *argv[MAX_OP_NUM]) {
	int i = 0;
	while (i < MAX_OP_NUM)
		printf("%s, ", argv[i++]);

	putchar('\n');
}


