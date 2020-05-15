#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "def.h"

#define TRUE 1
#define FALSE 0

/*label data type. includes vars for label name, label type, if label is used as entry, address in memory,pointer to operandNode list which contains all operands' IC values that are using this label (in use only for external labels)*/
typedef struct {
	char name[MAX_LBL_SZ + 1]; /* because there is '/0' in the end */
	unsigned int isData: 1;
	unsigned int isCode: 1;
	unsigned int isEntry: 1;
	unsigned int isExtern: 1;
	int value;
} symbol;

typedef struct symbolNode {
	symbol data;
	struct symbolNode *next;
} symbolNode;

/* pointer for the symbol table (point on node in the table) */
symbolNode *head = NULL;

symbol *symbolInTable(char *label) {
	symbolNode *ptr = head;

	/* scan the table and compare the label to the previous labels */
	while (ptr != NULL) {
		if (!strcmp(label, ptr->data.name))
			return &(ptr->data);
	}
	return NULL; /* symbol is not in table */
}

int addSymbol(char *label, int guide) {
	symbolNode *ptr = head, *newNode;
	symbol data;

	/* check if symbol is already in the table */
	if (symbolInTable(label)) {
		printf("error in %d: label is already defined", lineNumber);
		return -1; /* error code */
	}

	/* create new node */
	strcpy(data.name, label);
	data.isData = ((guide == DATA) || (guide == STR));
	data.isEntry = (guide == ENTRY);
	data.isExtern = (guide == EXTERN);
	data.isCode = ((guide == ENTRY) || (guide == EXTERN) || (guide == CODE));
	data.value = (data.isData) ? DC : IC;

	newNode->data = data;
	newNode->next = NULL;

	/* go to the end of the list */
	while (ptr->next != NULL)
		ptr = ptr->next;

	ptr->next = newNode;
	return 0; /* success code */
}

int getValue(char *label) {
	symbol *S = symbolInTable(label);
	if (S == NULL)
		return -1; /* symbol is not in table */

	return S->value;
}

int isSymbolExternal(char* label) {
	symbol *S = symbolInTable(label);
	if (S == NULL)
		return -1; /* symbol is not in table */

	return S->isExtern;
}

void updateDataLabels(int ICF) {
	symbolNode *ptr = head->next;

	/* add the offset for all data symbols */
	while (ptr) {
		if (ptr->data.isData)
			ptr->data.value += ICF;
	}
}


/*when an entry label is found in the second pass, 
this function will search for it in the label list and mark it as an antry label */
int makeEntry( char *labelName){
	
	symbolNode *ptr = head;

	while(ptr){
		if( !strcmp(ptr->data.name, labelName)){
			ptr->data.isEntry = 1;
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}
