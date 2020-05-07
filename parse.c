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


/* saves data guidance information to dataImage. expects pointer to string representation of data value 
 * INPUT: a line
 * OUTPUT: integer. -1 in case of an eror, 0 otherwise
 */
int getNumbers(char* line){

	char *token;
	char *endChar = NULL; 
	int tempDC = DC;
	int tempNum;

	if(!checkComma(line))
	{
		printf("Error in line %d: Data guidance parameter is incorrect, aborting storage action\n", lineNumber);
		DC = tempDC;
		return -1;
	}

	token = strtok(line,","); /* brakes the string to tokens of possible integers */
	while(token)
	{	
		tempNum = (int) strtol(token, &endChar, 10); /* 10 for decimal base */

		if(*endChar != '\0') /* the token doesn't ends with a digit */
		{
			printf("Error in line %d: Could not parse natural number in parameter %s\n", lineNumber, token);
			DC = tempDC;
			return -1;
		}
		if(tempNum >= 8388608 || tempNum < -8388608 ) /* the posible range for 2^24 bits in every word of memory */
		{
			printf("Error in line %d: Natural number parameter %s is too large to fit in memory\n", lineNumber, token);
			DC = tempDC;
			return -1;
		}
		dataImage[DC] = tempNum; /* stores the number in the data image */
		DC++;
		token = strtok(NULL,",");
	}
	return 0;
}
	
/* saves string guidance information to dataImage. expects pointer to string value 
 * INPUT: a line
 * OUTPUT: integer. -1 in case of an eror, 0 otherwise
 */
int getString(char* line){

	int i=0;

	while(str[i] != '\0')
	{
		dataImage[DC] = str[i]; /* stores the char in the data image  */
		i++;
		DC++;
	}
	dataImage[DC] = '\0'; 
	DC++;
	return 0;
}

/*check correct comma placement in string, expects pointer to string*/
int checkComma(char *str)
{
	if(*str == ',') /* if the string starts with comma sign */
	{
		printf("Error in line %d: Comma is dispositioned\n", lineNumber);
		return 0;
	}
	str++;
	while(*str)
	{
		if(*str == ',')
		{
			if(*(str+1) == '\0') /* comma in the end of sentence. e.g: "abc,de," */
			{
				printf("Error in line %d: Comma is dispositioned\n", lineNumber);
				return 0;
			}
			if(*(str+1) == ',') /* two or more comma signs in row. e.g: "abc, ,de" */
			{
				printf("Error in line %d: Multiple consecutive commas\n", lineNumber);
				return 0;
			}
		}
		str++;
	}
	return 1;
}
