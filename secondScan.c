#include <stdio.h>

#include "secondScan.h"
#include "symbolTable.h"
#include "machineCode.h"

/* secondScan - "scan" the file in the second time. in practice, doesn't scan the file but the data structures from the first scan
 * mark entry labels as entry, resolve symbols in file image from symbol table
 * save the lines that has external reference for extern file
 * INPUT: void (data structure saved between first and second scan, and the scan use only their abstractions
 * OUTPUT: status code - 0 if success, ERROR (-1) if an error encountered
 * METHOD: based on the algorithm in the book, but no re-parsing of the file. the similar step is written in comment for each part of the code
 */
int secondScan(void) {

	int symbolValue, errorFlag = 0;
	int IC, lastCommandIC = 0;
	char *symbol;

	printf("strating second scan...\n");

	/* all entries saved in list during the first scan. now go through the list and mark all the labels as entries */
	/* similar to step 5 in the algorithm from the notebook */
	if (entrifyList() < 0)
		errorFlag = ERROR_CODE;
	freeEntryList();

	/* check if need to add a symbol. if not, skip */
	for (IC = 0; IC < ICF; IC++) {

		if (getLineType(IC) == HEADER_LINE) {
			lastCommandIC = IC;
			lineNumber = getLineNumber(IC);
		}
		else {
			if ((symbol = getSymbol(IC)) != NULL) { /* check if this line contains a symbol */
				symbolValue = getValueFromSymbol(symbol);

				/* symbol in symbol table (and not external). similar to step 7 */
				if (symbolValue > 0)
					setSymbolValue(IC, lastCommandIC ,symbolValue); /* update value */

				/* symbol is external (only external symbol has value 0, otherwise should be MMRY_OFFSET or greater)
				 * similiar to step 6 */
				else if (symbolValue == 0) {
					setExternSymbol(IC);
					addExternNode(symbol, IC);
				}

				/* symbol is not in symbol table */
				else {
					printf("error in %d: %s: symbol not found\n", lineNumber, symbol);
					errorFlag = ERROR_CODE;
				}

				free(symbol); /* free space allocation for symbol, not needed anymore */
			}
		}
	}

	if (errorFlag == ERROR_CODE)
		printf("second scan failed\n");
	else
		printf("second scan finished successfully\n");

	return errorFlag;
}
