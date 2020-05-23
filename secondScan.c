#include <stdio.h>

#include "secondScan.h"
#include "parse.h"
#include "symbolTable.h"
#include "machineCode.h"

externNode *headExt = NULL;

int secondScan(FILE *fp , char * fileName) {

	int symbolValue, errorFlag = 0;
	int IC, lastCommandIC = 0;
	char *symbol, *entrySymbol ; 
	char *currLine = calloc(MAX_LN_LEN + 2, sizeof(char)); /* the current line text */;

	printf("strating second scan...\n");

	/* the part of the eneteris might not work */
	rewind (fp);
	lineNumber =0 ;
	while (fgets(currLine, MAX_LN_LEN + 1, fp)) { /* check for enteris */

		++lineNumber;
		if ( getGuideType(currLine) == ENTRY) {

			currLine = ( (getGuideType(currLine) == ENTRY ) ? strchr(currLine, 'y') + sizeof(char) : currLine);

			if ( (entrySymbol = getAnotherLabel(currLine)) == NULL) {
				printf("error in %d: entry command with no label\n", lineNumber );
				errorFlag = ERROR_CODE;
				continue;
			}

			if (  symbolInTable(entrySymbol) == NULL )  {
				printf("error in %d: label %s was not found", lineNumber, entrySymbol );
               			errorFlag = ERROR_CODE;
                		continue ;
            		}
           		makeEntry ( entrySymbol ); 
		}	
	} 


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

