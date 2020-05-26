#include <stdio.h>
#include "def.h"

#define TRUE 1
#define FALSE 0

/* addSymbol - add symbol to symbol table
 * INPUT: a label, guide type (for symbol properties), instruction and data counters for value of the symbol
 * OUTPUT: status code - 0 is success, ERROR else
 */
int addSymbol(char *label, int guide, int IC, int DC);

/* isSymbolExternal - return if the input symbol is external
 * INPUT: a label
 * OUTPUT: 1 is symbol is extern, 0 if not, ERROR (-1) if symbol doesn't exist
 */
int isSymbolExternal(char* label);

/* updateDataLabels - after first scan, used to update the value of data symbols
 * INPUT: value of final instruction counter
 * OUTPUT: no output, but set the right value to all the data symbols in table
 */
void updateDataLabels(int ICF);

/* getValueFromSymbol - return the value of a symbol
 * INPUT: a label
 * OUTPUT: a value of the label, ERROR (-1) if symbol doesn't exist
 */
int getValueFromSymbol(char *label);

/* addEntryNode - belongs to the entry list: a list that contain all the entry labels that declared in the first scan.
 * INPUT: a label
 * OUTPUT: add label to entry list and return status code
 */
int addEntryNode(char *label);

/* entrifyList - set labels from entry list to marked as entry in the symbol table. called in the second scan
 * INPUT: void
 * OUTPUT: 0 if success, ERROR if the as an entry label that not in the symbol table
 */
int entrifyList();

/* freeEntryList - free the memory allocation of the entry list
 * INPUT: void
 * OUTPUT: entry list is empty, memory allocations free, head set to be NULL
 */
void freeEntryList();

/* addExternNode - belongs to the extern list: a list that contains all the lines in the file image that need an
 * INPUT: a label and its location in the file image
 * OUTPUT: void, but add the label into the extern list
 */
void addExternNode(char *label, int IC);

/* printExtern - print the extern list into .ext file
 * INPUT: a file pointer
 * OUTPUT: void (data printed to the file)
 */
void printExtern(FILE* fp);

/* printEntry - print the entry symbols from symbol tableinto .ent file
 * INPUT: a file pointer
 * OUTPUT: void (data printed to the file)
 */
void printEntry(FILE* fp);

/* freeSymbolTable - free the memory allocation of the symbol table
 * INPUT: void
 * OUTPUT: symbol table is empty, memory allocations free, head set to be NULL
 */
void freeSymbolTable(void);

/* needEntFile - return if entry file should be created
 * INPUT: void
 * OUTPUT: 1 if entry file should be created, 0 else
 */
int needEntFile(void);

/* needEntFile - return if extern file should be created
 * INPUT: void
 * OUTPUT: 1 if extern file should be created, 0 else
 */
int needExtFile(void);
