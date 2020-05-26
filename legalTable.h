/* initLegalTable - init the legal table (all data stored in file)
 * INPUT: no input. read data from file in known name
 * OUTPUT: init the legal table (an internal data structure that keep all the relevant data)
 */
int initLegalTable();

/* isCmdValid - check if command is valid and operands are valid
 * INPUT: a command (not assumed to be valid) and address method of op1, op2
 * OUTPUT: opCode and funct if valid (simple math manipulate allow other function to calcualte them from this value, ERROR_CODE (-1) if invalid
 */
int isCmdValid(char *cmd, int addOP1, int addOP2);
