#include <stdio.h>
#include "def.h"

#define TRUE 1
#define FALSE 0

int DC;

int addSymbol(char *label, int guide, int IC, int DC);

int getValue(char *label);

int isSymbolExternal(char* label);

void updateDataLabels(int ICF);

int getValueFromSymbol(char *label);

int addEntryNode(char *label);

int entrifyList();

int addExternNode(char *label, int IC);

void printExtern(FILE* fp);

void printEntry(FILE* fp);

int needEntFile(void);

int needExtFile(void);
