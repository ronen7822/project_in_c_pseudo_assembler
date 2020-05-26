#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "def.h"

/* distinguish between header and data lines */
#define HEADER_LINE 0
#define DATA_LINE 1

/* buildBinaryCode - codes an instruction into the instrcution image
 * INPUT: instrcution opfunct (contains the opcode and funct of the commands), args and their addressing methods
 * OUTPUT: number of words to add to instrctuion counter (e.g. return value of 2, means that a command hear and one arg was added
 * return ERROR (-1) of error encountered
 * METHOD: just assign the input to the struct and cal codeArg for assigning the args
 */
int buildBinaryCode(int opfunct, char* srcOper, char* destOper, int sourceAdd, int destAdd, int IC);

/* getLineType - return if a line is a header or data (code an argument)
 * INPUT: an index in instruction image
 * OUTPUT: the type of the line in this index
 */
int getLineType(int IC);

/* getSymbol - for data lines, if the data is a symbol to resolve, the symbol is saved in the line index during the first scan.
 * this function return the symbol for a line (if symbol is NULL, there is no symbol and second scan skip this line)
 * INPUT: an index in instruction image
 * OUTPUT: symbol in this line
 */
char *getSymbol(int IC);

/* setSymbolValue - assign the value of symbol (during second scan)
 * INPUT: an index in instruction image (IC), index of the command in instruction image, value of a symbol
 * OUTPUT: void, but assign the value of a the symbol in the instruction image
 * METHOD: for DIR method, assign the value of the symbol, for REL method, assign the difference between header IC (lastCommandIC) and symbol value
 */
void setSymbolValue(int IC, int lastCommandIC, int symbolValue);

/* setExternSymbol - for symbol that detected to be external, set the refrence bits to represent that this symbol is external
 * INPUT: an index in instruction image to set to be external
 * OUTPUT: void, but reference bits in the line marked as external
 */
void setExternSymbol(int IC);

/* getLineNumber - for header lines, this function return the number of the line in the original file (".as" file) for error prints
 * INPUT: an index in instruction image
 * OUTPUT: the line number in the original file that corresponds to this index in instruction image
 */
int getLineNumber(int IC);

/* lineForPrint - calculate the value of a line, to print it in hex in the output file
 * INPUT: an index in instruction image
 * OUPUT: a numeric value of this line
 */
int lineForPrint(int IC);

