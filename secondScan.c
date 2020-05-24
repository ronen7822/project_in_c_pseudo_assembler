#include <stdio.h>

#include "secondScan.h"
#include "parse.h"
#include "symbolTable.h"
#include "machineCode.h"

externNode *headExt = NULL;

int secondScan(FILE *fp , char * fileName) {

	int symbolValue, errorFlag = 0;
	int IC, lastCommandIC = 0;
	char *symbol;

	printf("strating second scan...\n");

	/* all entries saved in list during the first scan. now go through the list and mark all the labels as entries */
	/* similar to step 5 in the algorithm from the notebook */
	if (entrifyList() < 0)
		errorFlag = ERROR_CODE;

	/* check if need to add a symbol. if not, skip */
	for (IC = 0; IC <= ICF; IC++) {

		if (getLineType(IC) == HEADER_LINE) {
			lastCommandIC = IC;
			lineNumber = getLineNumber(IC);
		}
		else {
			if ((symbol = getSymbol(IC)) != NULL) { /* check if this line contains a symbol */

				if ( (symbolValue = getValueFromSymbol(symbol)) > 0) /* get symbol from symbol table if not external . ERROR if no such symbol */
					setSymbolValue(IC, lastCommandIC ,symbolValue); /* update value */

				else if ( isSymbolExternal(symbol) > 0 ) {
					setExternSymbol(IC) ; /* need to build list of externals */
					addExternNode(IC, symbol);
				}
				else {
					printf("error in %d: symbol (%s) not found\n", lineNumber, symbol);
					errorFlag = ERROR_CODE;
				}
				free(symbol); /* free space allocation for symbol, not needed anymore */
			}
		}
	}

	if (errorFlag == ERROR_CODE)
		return ERROR_CODE; /* indicates that error was found*/


	return buildFiles(fileName , ICF, dataImage ) ;
}

int addExternNode(int lineNum, char *symbol) {
	
	externNode *newNode = (externNode*) calloc(1, sizeof(externNode));

	/* create new node */
	newNode->lineVal = lineNum+MMRY_OFFSET;
	strcpy((newNode->name), symbol);

	newNode->next = headExt;
	headExt = newNode; 

	return 1;
}

/* return the next external node*/
externNode * findExternalLabel(externNode *ptr) {
	return ptr->next;
}

int freeExternList(void) {

	externNode *ptr = headExt;
	externNode *temp = headExt;
	/* add the offset for all data symbols */
	while (ptr != NULL) {

		temp = ptr->next;
		free(ptr);
		ptr = temp;
	}
	
	return 1;
}


