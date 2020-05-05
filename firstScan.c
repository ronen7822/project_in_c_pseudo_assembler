#include "def.h"
#include "legalTable.c"
#include "parse.c"
#include "symbolTable.c"
/* #include "machineCode.c" */

#include <string.h>


int calcWordNum(int op1, int op2);

/* the first scan in the dual scan algorithm, no header file for now, could be added in the future */
int first_scan(FILE *fp)
{
	char *currLine = calloc(MAX_LN_LEN, sizeof(char)); /* the current line text */
	char *labelName = calloc(MAX_LN_LEN, sizeof(char)); /* holds the label, if exist. can hold a full line - for error detection */
	char* argv[MAX_OP_NUM]; /* holds arguments for command statements */

	int op1Add, op2Add, argc, wordNum;
	int errorFlag = 0, symbolFlag; /* flags indicate on errors, symbol in line */
	int contInd; /* index of the start of the statement after the label */

	enum guideType guideType; /* used to hold type of guidance for guidance statement */

	union dataContent { /* holds data for data image */
		int data[(MAX_LN_LEN - strlen(".data ")) / 2 + 1]; /* maximum numbers in one line */
		char string[MAX_LN_LEN - strlen(".string ") - 2]; /* longest string */
	} dataContent;

	extern int DCF; /* data counter final */
	extern int ICF; /* instruction counter final */

	rewind (fp); /* sets the pointer to the begining of the file*/

	/* reset IC, DC. TODO: why do they external? */
	DC = 0;
	IC = MMRY_OFFSET;

	while (fgets(currLine, MAX_LN_LEN + 1, fp)) { /* stages 2-16 */
		symbolFlag = 0; /* reset symbolFlag */
		++lineNumber;

		/* skip comments */
		if (currLine[0] == ';')
			continue;

		/* look for symbols - stages 3,4 */
		if ((labelName = getLabel(currLine)) != NULL) {
			if (labelValid(labelName) < 0)
				errorFlag = -1;
			symbolFlag = 1;
		}


		/* set line pointer to be right after the label (the label is irrelevant for the rest of the line) */
		currLine = ((labelName != NULL) ? strchr(currLine, ':') + sizeof(char) : currLine);

		if ((guideType = getGuideType(currLine)) < 0)
			errorFlag = -1; /* error code */
		else if (guideType > 1) { /* 0 mean no guide in this line */

			/* stage 9. if guideType is ".entry", it will be defined in 2nd scan */
			if (guideType == ENTRY) {
				if (labelName != NULL)
					printf("warning in %d: a label in entry statement is meaningless", lineNumber);
				continue;
			}

			/* stage 10: get label og extern statement */
			if (guideType == EXTERN) {
				if (labelName != NULL)
					printf("warning in %d: a label in extern statement is meaningless", lineNumber);

				/* parse new label name to add to symbol table */
				labelName = getAnotherLabel(currLine + strlen(".extern ")*sizeof(char));
				symbolFlag = 1;
			}

			/* if there is a symbol add symbol to symbol table */
			if ((symbolFlag) && (addSymbol(labelName, guideType) < 0))
				errorFlag = -1;

			/* stages 5-7 */
			if (guideType == DATA) {
				/* parse and add data */
			}
			else if (guideType == STR) {
				/* parse and add string */
			}

			continue; /* in stages 7,10 we back to level 2 */
		}

		/* stage 11 */
		if (symbolFlag)
			addSymbol(labelName, CODE);

		argc = parseCommand(argv, currLine);
		op1Add = getAddMthd(argv[1]);
		op2Add = getAddMthd(argv[2]);

		/* check command validity: command exist and operands are in right adreesing method */
		if (isCmdValid(argv[0], op1Add, op2Add) < 0) /* להדפיס שגיאה גם עבור האופרנד השני */
			errorFlag = -1; /* error code */

		wordNum = calcWordNum(op1Add, op2Add);

		/* build binary code */

		/* precede IC */
		IC += wordNum;
	}

	/* errorFlag is set to 1 if an error detected in the scan, stage 17 */
	if (errorFlag < 0)
		return -1; /* indicates that eror was found*/

	DCF = DC; /* stage 18 */
	ICF = IC;

	updateDataLabels(ICF); /* adds ICF value to all the lablels in the symbol list - stage 19 */

	return 0; /* success code */
}

