#include "legalTable.h"
#include "parse.h"
#include "symbolTable.h"
#include "machineCode.h"

#include <string.h>
#include <stdlib.h>

int ICF;
int DCF;
int lineNumber = 0;

/* initialize data image out of function because it is extern variable  */
dataNode dataImage ;


static void replaceArgs(char *arg1, char *arg2);

/* the first scan in the dual scan algorithm, no header file for now, could be added in the future */
int firstScan(FILE *fp) {
	char *currLine = calloc(MAX_LN_LEN + 2, sizeof(char)); /* the current line text */
	char *labelName = calloc(MAX_LN_LEN + 1, sizeof(char)); /* holds the label, if exist. can hold a full line - for error detection */
	char* argv[MAX_OP_NUM]; /* holds arguments for command statements */

	int op1Add, op2Add, opfunct, i;
	int errorFlag = 0, symbolFlag; /* flags indicate on errors, symbol in line */

	enum guideType guideType; /* used to hold type of guidance for guidance statement */

	/* reset IC, DC */
	int IC = 0;
	int DC = 0;

	dataNode* dataImagePtr = &dataImage;
	dataImagePtr->length = 0 ;
	dataImagePtr->data.intPtr = NULL ;
	dataImagePtr->next = NULL ;

	/* memory allocation for argv */
	for (i = 0; i < MAX_OP_NUM; i++)
		argv[i] = (char*) calloc(MAX_LN_LEN + 1, sizeof(char));

	rewind (fp); /* sets the pointer to the begining of the file*/

	printf("strating first scan...\n");
	while (fgets(currLine, MAX_LN_LEN + 1, fp)) { /* stages 2-16 */

		symbolFlag = 0; /* reset symbolFlag */
		++lineNumber;

		/* skip comments and blank spaces*/
		if (currLine[0] == ';')
			continue;

		/* look for symbols - stages 3,4 */
		if ((labelName = getLabel(currLine)) != NULL) {
			if (labelValid(labelName) < 0)
				errorFlag = ERROR_CODE;
			symbolFlag = 1;
		}

		/* set line pointer to be right after the label (the label is irrelevant for the rest of the line) */
		currLine = ((labelName != NULL) ? strchr(currLine, ':') + sizeof(char) : currLine);

		if ((guideType = getGuideType(currLine)) < 0)
			errorFlag = ERROR_CODE; /* error code */

		else if (guideType > 0) { /* 0 mean no guide in this line */

			/* stage 9. if guideType is ".entry", it will be defined in 2nd scan */
			if (guideType == ENTRY) {
				if (labelName != NULL) {
					printf("warning in %d: a label in entry statement is meaningless\n", lineNumber);
					if (addSymbol(labelName, guideType, IC, DC) < 0) /* label in entry statement is not an error and need to be in symbol table */
						errorFlag = ERROR_CODE;
				}

				/* add the label to the entry list; in the second scan will set all the symbols to be entries */
				labelName = getAnotherLabel(currLine + strlen(".entry ")*sizeof(char));
				if (addEntryNode(labelName) < 0)
					errorFlag = ERROR_CODE;
				continue;
			}

			/* stage 10: get label og extern statement */
			if (guideType == EXTERN) {
				if (labelName != NULL) {
					printf("warning in %d: a label in extern statement is meaningless\n", lineNumber);
					if (addSymbol(labelName, guideType, IC, DC) < 0) /* label in extern statement is not an error and need to be in symbol table */
						errorFlag = ERROR_CODE;
				}

				/* parse new label name to add to symbol table */
				labelName = getAnotherLabel(currLine + strlen(".extern ")*sizeof(char));
				symbolFlag = 1;
			}

			/* if there is a symbol add symbol to symbol table */
			if ((symbolFlag) && (addSymbol(labelName, guideType, IC, DC) < 0))
				errorFlag = ERROR_CODE;

			/* stages 5-7 */
			if (guideType == DATA) {
				if ((dataImagePtr->next = getNumbers(currLine + strlen(".data ")*sizeof(char))) != NULL) {
					dataImagePtr = dataImagePtr->next;
					DC += dataImagePtr->length;
				}
				else
					errorFlag = ERROR_CODE;
			}
			else if (guideType == STR) {
				if ((dataImagePtr->next = getString(currLine + strlen(".string ")*sizeof(char))) != NULL) {
					dataImagePtr = dataImagePtr->next;
					DC += dataImagePtr->length;
				}
				else
					errorFlag = ERROR_CODE;
			}

			continue; /* in stages 7,10 we back to level 2 */
		}

		/* stage 11 */
		if (symbolFlag)
			addSymbol(labelName, CODE, IC, DC);

		if (parseCommand(argv, currLine) < 0) {
			errorFlag = ERROR_CODE; /* errors are printed in the function */
			continue;
		}

		if (*(argv[0]) == '\0')
			continue;

		replaceArgs(argv[1], argv[2]); /* if only one argument exist, it should be arg2 and not arg1 */
		op1Add = getAddMthd(argv[1]);
		op2Add = getAddMthd(argv[2]);

		/* check command validity: command exist and operands are in right adreesing method */
		if ((opfunct = isCmdValid(argv[0], op1Add, op2Add)) < 0)
			errorFlag = ERROR_CODE; /* error code */
		else
			IC += buildBinaryCode(opfunct, argv[1], argv[2], op1Add, op2Add, IC); /* build instruction image and precede IC */

	}


	printf("IC = %d, DC = %d\n", IC, DC);

	/* errorFlag is set to 1 if an error detected in the scan, stage 17 */
	if (errorFlag == ERROR_CODE)
		return ERROR_CODE; /* indicates that error was found*/


	DCF = DC; /* stage 18 */
	ICF = IC + MMRY_OFFSET;

	updateDataLabels(ICF); /* adds ICF value to all the lablels in the symbol list - stage 19 */
	return 0; /* success code */
}

static void replaceArgs(char *arg1, char *arg2) {
	if (*arg2 == '\0') { /* arg2 == 0: arg2 is empty */
		strcpy(arg2, arg1); /* copy arg1 to arg2 */
		*arg1 = '\0'; /* remove arg1 by replacing its first char by 0 */
	}
}

