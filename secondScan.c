#include <stdio.h>

#include "secondScan.h"
#include "parse.h"
#include "symbolTable.h"
#include "machineCode.h"

int secondScan(void) {

	int symbolValue, errorFlag = 0;
	int IC, lastCommandIC = 0;
	char *symbol;

	printf("strating second scan...\n");

	/* all entries saved in list during the first scan. now go through the list and mark all the labels as entries */
	/* similar to step 5 in the algorithm from the notebook */
	if (entrifyList() < 0)
		errorFlag = ERROR_CODE;

	/* check if need to add a symbol. if not, skip */
	for (IC = 0; IC < ICF; IC++) {

		if (getLineType(IC) == HEADER_LINE) {
			lastCommandIC = IC;
			lineNumber = getLineNumber(IC);
		}
		else {
			if ((symbol = getSymbol(IC)) != NULL) { /* check if this line contains a symbol */
				symbolValue = getValueFromSymbol(symbol);

				/* symbol in symbol table (and not external) */
				if (symbolValue > 0)
					setSymbolValue(IC, lastCommandIC ,symbolValue); /* update value */

				/* symbol is external (only external symbol has value 0, otherwise should be MMRY_OFFSET or greater */
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

	return errorFlag;
}
