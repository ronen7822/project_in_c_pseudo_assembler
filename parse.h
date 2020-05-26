#include "def.h"

/* getLabel - extract a label from a line
 * INPUT: a line
 * OUTPUT: a string (char*) of the label, or NULL if there is no labal in the line
 * METHOD: return the part in the line from the first non-white character space until ':'
 */
char *getLabel(char *line);

/* getLabel - extract a label after entry or extern guide, from a line
 * INPUT: a line (after ".entry" or ".extern" guide was encountered)
 * OUTPUT: a string (char*) of the label, or NULL if there is no labal in the line
 */
char *getAnotherLabel(char *line);

/* labelValid - check validity of label
 * INPUT: a label
 * OUTPUT: validity of the label (start with a letter, contains only nums and letters, in a proper length)
 */
int labelValid(char *label);

/* getGuideType - check if the line contain guide statement, if it is, return its type
 * INPUT: a line to parse
 * OUTPUT: type of guide (NO_GUIDE if not a guidance statement)
 */
int getGuideType(char* line);

/* getAddMthd - analize the operand to get its addressing method
 * INPUT: an operand (as string)
 * OUTPUT: indication of its addressing method (NON = no operand)
 */
int getAddMthd(char* op);

/* getNumbers - extracts numbers into array of integers inside a dataNode
 * INPUT: a part of line (after ".data" guide encountered) to parse
 * OUTPUT: a node with data for success, NULL in case of error
 */
dataNode *getNumbers(char* line);

/* getStirng - extracts string into array of chars inside a dataNode
 * INPUT: a part of line (after ".string" guide encountered) to parse
 * OUTPUT: a node with data for success, NULL in case of error
 */
dataNode *getString(char* line);

/* parseCommand - take command and extract the args
 * parseCommand check the command syntax (commas, no more than 3 args, right use in commas, etc.)
 * but NOT the command context (correct command to run, number of args correlate to command, types of args etc.)
 * INPUT: a command (cmd) and array to store the arguments
 * OUTPUT: argument count (argc), ERROR_CODE for invalid syntax. the arguments in argv
 */
int parseCommand(char *argv[MAX_OP_NUM], char *cmd);

/* printArgv - aid function (not in use). print argv - usefull for debugging
 * INPUT: argv
 * OUTPUT: print argv
 */
void printArgv(char *argv[MAX_OP_NUM]);
