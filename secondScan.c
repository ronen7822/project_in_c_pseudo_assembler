#include <stdio.h>

#include "def.h"
#include "symbolTable.h"
#include "machineCode.h"

int secondScan(FILE *fp) {

	int symbolValue, errorFlag = 0;
	int IC, lastCommandIC = 0;
	char *symbol;

	printf("strating second scan...\n");

	/* entry lines */

	/* extern lines */

	/* check if need to add a symbol. if not, skip */
	for (IC = 0; IC <= ICF; IC++) {
		if (getLineType(IC) == HEADER_LINE) {
			lastCommandIC = IC;
			lineNumber = getLineNumber(IC);
		}
		else {
			if ((symbol = getSymbol(IC)) != NULL) { /* check if this line contains a symbol */
				if ((symbolValue = getValueFromSymbol(symbol)) > 0) /* get symbol from symbol table. ERROR if no such symbol */
					setSymbolValue(IC, lastCommandIC ,symbolValue); /* update value */
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

	return 0;
}
