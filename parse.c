#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "parse.h"
#include "def.h"

#define MAX_GUIDE_LEN 6 /* maximum length of guidance ("string" is the longest) */
#define MIN(a,b) (a < b) ? (a) : (b)

/* getLabel - extract a label from a line
 * INPUT: a line
 * OUTPUT: a string (char*) of the label, or NULL if there is no labal in the line
 * METHOD: return the part in the line from the first non-white character space until ':'
 */
char *getLabel(char *line) {
	char *label = calloc(MAX_LN_LEN + 1, sizeof(char));
	char *startLabel, *endLabel;

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



/* getStirng - extracts the numbers into array of chars inside dataContent
 * INPUT: a line and dataContent union
 * OUTPUT: integer. -1 in case of an eror, 0 otherwise
 */
int getNumbers(char* line, dataContent* array){

	char *token;
	char *endChar = NULL; 
	int tempNum, i = 0;

	while( line[i] == ' ' || line[i] == '\t' ) /* skips white spaces */
		i++;

	if( line[i] == ",")
	{
		printf("Error in line %d: Data guidance parameter is incorrect, aborting storage action\n", lineNumber);
		return -1; /* eror code */
	}

	token = strtok(line,","); /* brakes the string to tokens of possible integers */
	while(token)
	{	
		tempNum = (int) strtol(token, &endChar, 10); /* 10 for decimal base */

		if(*endChar != '\0') /* the token doesn't ends with a digit */
		{
			printf("Error in line %d: could not parse natural number in parameter %s\n", lineNumber, token);
			return -1;
		}

		if( tempNum >= pow(2, 23) || tempNum < pow(2, 23) ) /* the posible range for 2^24 bits in every word of memory */
		{
			printf("Error in line %d: natural number parameter %s is too large to fit in memory\n", lineNumber, token);
			return -1;
		}
		array.data[i] = tempNum; /* stores the number in the union */
		token = strtok(NULL,",");
		i++ ;
	}

	array.data[i] = 16777216; /* 2^24 - sign to end of numbers . use other method to represnt the end of the array*/
	return 0;
}
	
/* getStirng - extracts string into array of chars inside dataContent
 * INPUT: a line and dataContent union
 * OUTPUT: integer. -1 in case of an eror, 0 otherwise
 */
int getString(char* line, dataContent* array){

	int i=0;
	
	while( line[i] == ' ' || line[i] == '\t' ) /* skips white spaces */
		i++;

	if ( line[i] != '"') {
		printf("Error in line %d: absence of \" sign in start of %s\n", lineNumber, line );
		return -1; /* eror code */
	}
	
	while( line[i] != '"' ) {

		if ( !( line[i] >= 32 && line[i] < 126)){ /* checks wheter line[i] is invisible character  */
			printf("Error in line %d: could not parse %c into visible character  \n", lineNumber,line[i]);
			return -1;
		}	

		if ( line[i] == '\0' ) {
			printf("Error in line %d: absence of \" sign in end of %s\n", lineNumber, line );
			return -1;
		}
		array.string[i] = line[i]; /* stores the char in the union  */
		i++;
	}
	
	if ( line[i] != '\0' ) { /* extraneous text after '"' sign */
		printf("Error in line %d: extraneous text after end of string %s\n", lineNumber, line );
		return -1;
	}

	array.string[i] = '\0'; 
	return 0;
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
			errorFlag = 1;
		}

		/* CASE: comma. DO: same as white space but check for no double commas */
		else if (cmd[i] == ',') {
			if (iw == INWORD) { /* case of end of word */

				if ( (argc = addArg(cmd, argv, i, argStart, argc)) < 0 )
					errorFlag = 1; /* error code. error message printed in addArg */
				if (argc == MAX_OP_NUM ){
					printf("error in %d: extraneous text after end of command\n", lineNumber);
					errorFlag = 1;
				}
				ic = argc > 1 ? NEED_COMMA : FORBID_COMMA;
				iw = OUTWORD;
			}

			if (ic == FORBID_COMMA){ /* error: case of wrong comma */
				printf("error in %d: illegal comma\n", lineNumber);
				errorFlag = 1;
			}
			else if (ic == WAS_COMMA){ /* error: case of double commas */
				printf("error in %d: multiple consecutive commas\n", lineNumber);
				errorFlag = 1;
			}
			else if (ic == NEED_COMMA)
				ic = WAS_COMMA;

		}

		/* CASE: name of command, variable or a number */
		else if ( isalnum(cmd[i]) || cmd[i] == '+' || cmd[i] == '-') {

			if (ic == NEED_COMMA) {
				printf("error in %d: missing comma\n", lineNumber);
				errorFlag = 1; /* error code */
			}


			if (iw == OUTWORD) {
				iw = INWORD;
				argStart = i; /* the first char of the next arg */
			}
		}

		/* CASE: incalid character*/
		else {
			printf("error in %d: at %d: invalid character %c (ascii = %d)\n", lineNumber, i + 1, cmd[i], cmd[i]);
			errorFlag = 1;
		}
	}
	
	if ( errorFlag )
		return -1; /* error occurred */

	return argc; 
}


/* addArg - subfunction of parseCmd. Called to add argument into argv
 * if failed, return -1 if fail (too long arg), update arg count (argc) if succeed
 * INPUT: a command, indexes of the arg (argStart -> i), argv, argc
 * OUTPUT: argc value (-1 if fail)
 */
static int addArg(char* cmd, char *argv[MAX_OP_NUM ], int i, int argStart, int argc) {

	if (i - argStart > MAX_LBL_SZ)
		printf("error in %d: arg %d is too long, maximum length is %d\n",lineNumber, argc, MAX_LBL_SZ);
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
