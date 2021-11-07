#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"

/* symbol - contains all data for a symbol */
typedef struct {
	char name[MAX_LBL_SZ + 1]; /* because there is '/0' in the end */
	unsigned int isData: 1;
	unsigned int isCode: 1;
	unsigned int isEntry: 1;
	unsigned int isExtern: 1;
	int value;
} symbol;

/* symbolNode - a node in linked link that stores the symbol table data */
typedef struct symbolNode {
	symbol data;
	struct symbolNode *next;
} symbolNode;

/* entryNode - a node in linked list that save labels that appeared in .entry statment */
typedef struct entryNode {
	char name[MAX_LBL_SZ + 1];
	int lineNumber;
	struct entryNode *next;
} entryNode;

/* externNode - a node in linked list that saves all the appearences of extern labels in the file image */
typedef struct externNode {
	char name[MAX_LBL_SZ + 1];
	int IC;
	struct externNode *next;
} externNode;

/* pointer for the symbol table (point on node in the table) */
static symbolNode *head = NULL;
static entryNode *entHead = NULL;
static externNode *extHead = NULL;
static int isEntryFile = FALSE; /* do we need an entry file? set to be true if there is an entry statement */

/* symbolInTable - look for symnol in the symbol table
 * INPUT: a label
 * OUTPUT: the node in the symnol table that contain this label, or NULL if no such node
 */
static symbol *symbolInTable(char *label);

/* symbolInTable - look for symnol in the symbol table
 * INPUT: a label
 * OUTPUT: the node in the symnol table that contain this label, or NULL if no such node
 */
static symbol *symbolInTable(char *label) {
	symbolNode *ptr = head;

	/* scan the table and compare the label to the previous labels */
	while (ptr != NULL) {
		if (!strcmp(label, ptr->data.name))
			return &(ptr->data);
		ptr = ptr->next;
	}
	return NULL; /* symbol is not in table */
}

/* addSymbol - add symbol to symbol table
 * INPUT: a label, guide type (for symbol properties), instruction and data counters for value of the symbol
 * OUTPUT: status code - 0 is success, ERROR else
 */
int addSymbol(char *label, int guide, int IC, int DC) {
	symbolNode *newNode;
	symbol data;

	/* check if symbol is already in the table */
	if (symbolInTable(label)) {
		printf("error in %d: label is already defined\n", lineNumber);
		return ERROR_CODE;
	}

	/* check label length */
	if (strlen(label) > MAX_LBL_SZ) {
		printf("error in %d: label is too long\n", lineNumber);
		return ERROR_CODE;
	}

	/* create new node */
	newNode = (symbolNode*) malloc(sizeof(symbolNode));
	strcpy(data.name, label);
	data.isData = ((guide == DATA) || (guide == STR));
	data.isEntry = (guide == ENTRY);
	data.isExtern = (guide == EXTERN);
	data.isCode = ((guide == ENTRY) || (guide == EXTERN) || (guide == CODE));
	data.value = (data.isData) ? DC : ((data.isExtern) ? 0 : IC + MMRY_OFFSET);

	newNode->data = data;
	newNode->next = head;
	head = newNode; /* add the label to be the first in the list (order doesn't matter) */

	return 0; /* success code */
}


/* isSymbolExternal - return if the input symbol is external
 * INPUT: a label
 * OUTPUT: 1 is symbol is extern, 0 if not, ERROR (-1) if symbol doesn't exist
 */
int isSymbolExternal(char* label) {
	symbol *S = symbolInTable(label);
	if (S == NULL)
		return ERROR_CODE; /* symbol is not in table */

	return S->isExtern;
}

/* updateDataLabels - after first scan, used to update the value of data symbols
 * INPUT: value of final instruction counter
 * OUTPUT: no output, but set the right value to all the data symbols in table
 */
void updateDataLabels(int ICF) {
	symbolNode *ptr = head;
	/* add the offset for all data symbols */
	while (ptr != NULL) {

		if (ptr->data.isData)
			ptr->data.value += ICF;
		ptr = ptr->next;
	}
}

/* getValueFromSymbol - return the value of a symbol
 * INPUT: a label
 * OUTPUT: a value of the label, ERROR (-1) if symbol doesn't exist
 */
int getValueFromSymbol(char *label) {
	symbol *S = symbolInTable(label);
	if (S == NULL)
		return ERROR_CODE; /* symbol is not in table */

	return S->value;
}

/* addEntryNode - belongs to the entry list: a list that contain all the entry labels that declared in the first scan.
 * INPUT: a label
 * OUTPUT: add label to entry list and return status code
 */
int addEntryNode(char *label) {
	entryNode *newNode;

	/* check label length */
	if (strlen(label) > MAX_LBL_SZ) {
		printf("error in %d: label is too long\n", lineNumber);
		return ERROR_CODE;
	}

	/* create new node and set its data */
	newNode = (entryNode*) malloc(sizeof(entryNode));
	strcpy(newNode->name, label);
	newNode->lineNumber = lineNumber;
	newNode->next = entHead;
	entHead = newNode; /* add the label to be the first in the list (order doesn't matter) */

	return 0; /* success code */
}

/* entrifyList - set labels from entry list to marked as entry in the symbol table. called in the second scan
 * INPUT: void
 * OUTPUT: 0 if success, ERROR if the as an entry label that not in the symbol table
 */
int entrifyList(void) {
	entryNode *entPtr = entHead; /* used to free the entry list */
	symbol *S;
	int errorFlag = 0;

	while (entPtr != NULL) {
		/* search label in symbol table. if not exist, print error and return error code */
		S = symbolInTable(entPtr->name);
		if (S == NULL) { /* symbol doesn't exist */
			printf("error in %d: a label (%s) defined as entry but never defined\n", entPtr->lineNumber, entPtr->name);
			errorFlag = ERROR_CODE;
		}
		else {
			isEntryFile = TRUE; /* mark that there is need to output .ent file */
			S->isEntry = 1; /* set symbol to be entry */
		}

		/* go to the next node */
		entPtr = entPtr->next;
	}

	return errorFlag;
}

/* freeEntryList - free the memory allocation of the entry list
 * INPUT: void
 * OUTPUT: entry list is empty, memory allocations free, head set to be NULL
 */
void freeEntryList() {
	entryNode *entPtr = entHead, *forFree; /* used to free the entry list */

	while (entPtr != NULL) {
		forFree = entPtr;
		entPtr = entPtr->next;
		free(forFree);
	}

	entHead = NULL;
}

/* addExternNode - belongs to the extern list: a list that contains all the lines in the file image that need an
 * INPUT: a label and its location in the file image
 * OUTPUT: void, but add the label into the extern list
 */
void addExternNode(char *label, int IC) {
	externNode *newNode = (externNode*) malloc(sizeof(entryNode));

	/* create new node and set its data */
	strcpy(newNode->name, label);
	newNode->IC = IC;
	newNode->next = extHead;
	extHead = newNode; /* add the label to be the first in the list (order doesn't matter) */
}

/* printExtern - print the extern list into .ext file
 * INPUT: a file pointer
 * OUTPUT: void (data printed to the file)
 */
void printExtern(FILE* fp) {
	externNode *extPtr = extHead, *forFree; /* used to free the extern list */

	while (extPtr != NULL) {
		/* print line to file (one line per extern occurance) */
		fprintf(fp, "%s %07d\n", extPtr->name, extPtr->IC + MMRY_OFFSET);

		/* go to the next node and free the previous line */
		forFree = extPtr;
		extPtr = extPtr->next;
		free(forFree);
	}

	extHead = NULL;
}

/* printEntry - print the entry symbols from symbol tableinto .ent file
 * INPUT: a file pointer
 * OUTPUT: void (data printed to the file)
 */
void printEntry(FILE* fp) {
	symbolNode *symbolPtr = head; /* used to free the symbol table */

	while (symbolPtr != NULL) {
		/* print line to file (one line per extern occurance) */
		if (symbolPtr->data.isEntry)
			fprintf(fp, "%s %07d\n", symbolPtr->data.name, symbolPtr->data.value);

		/* go to the next node and free the previous line */
		symbolPtr = symbolPtr->next;
	}
}

/* freeSymbolTable - free the memory allocation of the symbol table
 * INPUT: void
 * OUTPUT: symbol table is empty, memory allocations free, head set to be NULL
 */
void freeSymbolTable() {
	symbolNode *symbolPtr = head, *forFree; /* used to free the symbol table */

	while (symbolPtr != NULL) {
		forFree = symbolPtr;
		symbolPtr = symbolPtr->next;
		free(forFree);
	}
	head = NULL; /* reset the symbol table */
}

/* needEntFile - return if entry file should be created
 * INPUT: void
 * OUTPUT: 1 if entry file should be created, 0 else
 */
int needEntFile(void) {
	return isEntryFile;
}

/* needEntFile - return if extern file should be created
 * INPUT: void
 * OUTPUT: 1 if extern file should be created, 0 else
 */
int needExtFile(void) {
	return (extHead != NULL);
}

