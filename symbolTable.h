#include "def.h"

#define TRUE 1
#define FALSE 0

int DC;

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

extern symbolNode *head;



int addSymbol(char *label, int guide, int IC, int DC);

int getValue(char *label);

int isSymbolExternal(char* label);

void updateDataLabels(int ICF);

int getValueFromSymbol(char *label);

int makeEntry(char *label);

symbol *symbolInTable(char *label);

symbolNode * findEntryLabel(symbolNode *ptr);

int freeList(void);

