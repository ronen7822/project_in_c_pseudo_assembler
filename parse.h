#include "def.h"


/* check if the n'th word in a command line is label */
int is_symbol( char*, int ) ;

/* check if the n'th word in a command line is guidance word (.data or .string) */
int is_guidance( char*, int );

/* check if the n'th word in a command line is the guidance word (.extern) */
int is_extern(char*, int);

/* check if the n'th word in a command line the is guidance word (.entry) */
int is_entry(char*, int);

/* check if the adrresing method of the n'th word in a command line is immidiate. return 1 if yes, 0 otherwise  */
int immidiate_addressing(char*, int);

/* extracrt the n'th word from a string, return the word if seccseed, '/0' otherwise */
char* extract_word(char*, int);


/* private methods */

/*check if a word in the assembly code is a label, return 1 if yes, 0 otherwise*/
static int isLabel(char *);	
	
/*check if a word in the assembly code is a guidance word (.data or .string)
,return index in guidanceTypes array if yes, 0 otherwise. expects pointer to string*/	
static int isGuidance(char *);	

/*check if a word in the assembly code is a guidance word (.extern),
 return index in guidanceTypes array if yes, 0 otherwise. expects pointer to string*/	
static int isExtern(char *);	

/*check if a word in the assembly code is a guidance word (.entry),
 return index in guidanceTypes array if yes, 0 otherwise. expects pointer to string*/	
static int isEntry(char *);

/*check if a word in the assembly code is given in immidiae addresing type, return 1 if yes, 0 otherwise*/
static int immidiateAddressing(char* );

/*check if a word in the assembly code is an instruction, return index in instructionTypes array if yes, 0 otherwise. expects pointer to string*/ 	
static int isInstruction(char *);
	
/*check if a word in the assembly code is a register name, return register number if yes, 0 otherwise. expects pointer to string*/ 			
static int isRegister(char *);							

/*checks if label name is legal according to specifications. expects pointers to label name*/
int isLegitLabelName(char *);	

/*saves data guidance information to dataImage. expects pointer to string representation of data value*/
int storeData(char *);												

/*check correct comma placement in string, expects pointer to string*/
int checkComma(char *);			
