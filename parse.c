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
