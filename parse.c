#include "parsing.h"
#include "def.h"

int is_symbol( char* str, int n )
{
	/* extract the n'th word from str*/
	char* token = extract_word(str, n);
	if ( isLegitLabelName(token) )
		return 1; /*  the n'th word in str is lable*/

        return 0 ; /*  the n'th word in str is not lable*/
}

int is_guidance( char* str, int n )
{
	/* extract the n'th word from str*/
	char* token = extract_word(str, n);
	if ( isGuidance(token) )
		return 1; /*  the n'th word in str is guidance word (.string or .data) */

        return 0 ; 
}

int is_extern( char* str, int n )
{
	/* extract the n'th word from str*/
	char* token = extract_word(str, n);
	if ( isExtern(token) )
		return 1; /*  the n'th word in str is the guidance word (.extern) */

        return 0 ; 
}

int is_entry( char* str, int n )
{
	/* extract the n'th word from str*/
	char* token = extract_word(str, n);
	if ( isEntry(token) )
		return 1; /*  the n'th word in str is the guidance word (.exntry) */

        return 0 ; 
}

int immidiate_addressing( char* str, int n)
{
	/* extract the n'th word from str*/
	char* token = extract_word(str, n);
	if ( immidiateAddressing(token) )
		return 1; /*  the n'th word in str is the guidance word (.exntry) */

        return 0 ; 
}

/* extracrt the n'th word from a string, return the word if seccseed, '/0' otherwise */
char* extract_word(char* str, int n)
{
	int i, j, k; /* index's */
	char * token[MAX_LBL_SZ] = "\0" ; 	

	if ( n == 0 )
 	{
		for ( i =0 ; (str[i] == ' ' || str[i] == '\t' ) ; i++) /*skip white spaces */
			;
		for ( j =i ; (str[i] != ' ' && str[i] != '\t' ) ; j++) /*skip non  white spaces */
			;
		memcpy(token, str[i], j-i); /* copy the first word in the sentence into the token*/
		return token;
	}
        /* get the first token */
        token = strtok(str, ", \t"); 
        k = 1 ;
	while( k < n && token != NULL )   
      		token = strtok(NULL, s); /* gets the n'th word in the sentence  */

        return token
}


static int isGuidance(char *str)
{
	int i;

	for (i=0; i<2 ; i++)
	{
		if( ! strcmp(guidanceTypes[i], str) ) /* if equal to ".data" or ".string" */
			return i+1 ;
	}
	return 0;
}

static int isExtern(char *str)
{
	if( ! strcmp(guidanceTypes[2], str) ) /* if equal to ".extern" */
			return i+1 ;
	return 0;
}

static int isEntry(char *str)
{
	if( ! strcmp(guidanceTypes[3], str) ) /* if equal to ".entry" */
			return i+1 ;
	return 0;
}

static int immidiateAddressing(char *str)
{
	if (str[0] == '#')
		return 1
	return 0
}

static int isInstruction(char *str)
{
	int i=0;

	while(instructionNames[i] != NULL)
	{
		if( !strcmp(instructionNames[i], str) )
			return i+1 ;
		i++;
	}
	return 0;
}


static int isRegister(char *str)
{
	int regNum;
	char *endChar = NULL; 
	
	if(str[0] == 'r')
	{
		regNum = (int) strtol(str+1, &endChar, 10); /* parse the string into integer */

		if(*endChar != '\0' || endChar == str+1)
			return 0;

		if( regNum>=0 && regNum<=7 )
			return regNum+1 ; /* legit register name */
	}
	return 0; /* not a  register name */
}

int isLegitLabelName(char *str)
{
	char temp[MAX_LABEL_SIZE]; /* holds copy of str */
	int i=0;

	if(!isalpha(*str)) /* checks whether a character is an alphabet or not  */
	{
		printf("Error in line %d: Label name starts with a digit\n", lineNumber);
		return 0;
	}
	while(*str != ':' && *str != '\0')
	{
		if(i > MAX_LBL_SZ)
		{
			printf("Error in line %d: Label name too long\n", lineNumber);
			return 0;
		}
		if(!isalpha(*str) && !isdigit(*str))
		{
			printf("Error in line %d: Label name contains a char which is not a digit nor a letter\n", lineNumber);
			return 0;
		}
		temp[i] = *str;
		str++;
		i++;
	}

	if(isRegister(temp)>=0 || isInstruction(temp)>=0) /* insure that str is not register name nor instruction name  */
	{
		printf("Error in line %d: Label name cannot be register or instruction name\n", lineNumber);
		return 0;
	}
	return i;
}

/* saves data guidance information to dataImage */
int storeData(char *str)
{
	char *token;
	char *endChar = NULL;
	int tempDC = DC;
	int tempNum;
	if(!checkComma(str))
	{
		printf("Error in line %d: Data guidance parameter is incorrect, aborting storage action\n", lineNumber);
		DC = tempDC;
		return 1;
	}
	token = strtok(str,",");
	while(token)
	{	
		tempNum = (int) strtol(token, &endChar, 10);
		if(*endChar != '\0')
		{
			printf("Error in line %d: Could not parse natural number in parameter %s\n", lineNumber, token);
			DC = tempDC;
			return 1;
		}
		if(tempNum >= 1048576 || tempNum < -1048576 ) /* there is 2^21 bytes in memeory */
		{
			printf("Error in line %d: Natural number parameter %s is too large to fit in memory\n", lineNumber, token);
			DC = tempDC;
			return 1;
		}
		dataImage[DC] = tempNum;
		DC++;
		token = strtok(NULL,",");
	}
	return 0;
}

/* saves string guidance information to dataImage */
int storeString(char *str)
{
	int i=0;

	while(str[i] != '\0')
	{
		dataImage[DC] = str[i];
		i++;
		DC++;
	}
	dataImage[DC] = '\0';
	DC++;
	return 0;
}

int checkComma(char *str)
{
	if(*str == ',')
	{
		printf("Error in line %d: Comma is dispositioned\n", lineNumber);
		return 0;
	}
	str++;
	while(*str)
	{
		if(*str == ',')
		{
			if(*(str+1) == '\0') /* comma in the end of sentence. e.g: "abc,de, */
			{
				printf("Error in line %d: Comma is dispositioned\n", lineNumber);
				return 0;
			}
			if(*(str+1) == ',') /* two or more comma signs in row. e.g: "abc, ,de */
			{
				printf("Error in line %d: Multiple consecutive commas\n", lineNumber);
				return 0;
			}
		}
		str++;
	}
	return 1;
}


int processInstruction(int insType, char *str)
{
	char *token1, *token2, *token3;
	instAddressType oper1 = NONE, oper2 = NONE;
	if(insType > 13)
	{
		if(str)
			printf("Error in line %d: Redundant parameters\n",lineNumber);

		else
			return(storeInstruction(insType, -1, -1));
	}
	else if(str)
	{
		if(!checkComma(str))
		{
			return 1;
		}
		token1 = strtok(str, ",");
		token2 = strtok(NULL, ",");
		token3 = strtok(NULL, ",");
		if(token1 && !token3)
		{
			oper1 = getOperandType(token1);
			if((insType >= 5 ))
			{
				if(token2)
					printf("Error in line %d: Too many operands\n",lineNumber);
				else
				{
					if(isDestOperandCompatible(oper1, insType))
						return(storeInstruction(insType, -1, oper1));
					else
						printf("Error in line %d: Destination operand is not compatible with instruction\n",lineNumber);
				}
			}
			else
			{
				if(token2)
				{
					if(isSourceOperandCompatible(oper1, insType))
					{
						oper2 = getOperandType(token2);
						if(isDestOperandCompatible(oper2, insType))
							return(storeInstruction(insType, oper1, oper2));
						else
							printf("Error in line %d: Destination operand is not compatible with instruction\n",lineNumber);
					}
					else
						printf("Error in line %d: Source operand is not compatible with instruction\n",lineNumber);
				}
				else
					printf("Error in line %d: Destination operand is missing\n",lineNumber);
			}
		}
		else
			printf("Error in line %d: Incorrect number of operands\n",lineNumber);
	}
	else
		printf("Error in line %d: Missing operands\n",lineNumber);

	return 1;
}

int storeInstruction(int insType, int source, int dest)
{
/* was not been implemented for now */
}

int isSourceOperandCompatible(int oper, int insType)
{
	if((insType <= 4 && insType >= 0) && ( oper == IMM || oper == DIR || oper == REG)) )
		return 1;
	if( insType == 5 && oper == DIR )
		return 1;
 	if( insType >= 6  )
		return 1;
	return 0 ;
}

int isDestOperandCompatible(int oper, int insType)
{
	/* all the data can be found in the legal addresing for operands table in page 46 */
	if(( (insType == 0) || (insType <= 9 && insType >= 2) && (insType==12) ) && ( oper == DIR || oper == REG))
		return 1;
	if((insType == 1 || insType == 13) && ( oper == IMM || oper == DIR || oper == REG)) )
		return 1;
	if((insType <= 11 && insType >= 9) && ( oper == DIR || oper == REL) )
		return 1;
	if(  insType >= 14 )
		return 1;
	return 0;
}


int getOperandType(char *str)
{
	if(str[0] == '#') /* for immidaite addressing */
		return IMM;
	if(str[0] == '&') /* for relative addressing */
		return REL;
	if(isRegister(str) >= 0)/* for register addressing */
		return REG;

	return DIR;/* for direct addressing */
}

void removeRedundantSpaces(char *buf)
{
	char *temp = buf;
	int comma = 0; int space = 0;
	while(*buf != EOF && *buf != '\n')
	{
		if(*buf == ' ' || *buf == '\t')
		{
			space = 1;
		}
		else if(*buf == ',')
		{
			if(comma == 0)
			{
				comma = 1;
			}
			else
			{
				*temp = ',';
				temp++;
				space = 0;
			}
		}
		else
		{
			if(comma == 1)
			{
				*temp = ',';
				temp++;
				comma = 0; space = 0;
			}
			else if(space == 1)
			{
				*temp = ' ';
				temp++;
				space = 0;
			}
			if(*buf == '"')
			{
				*temp = *buf;
				buf++;
				temp++;
				while(*buf != '"')
				{
					*temp = *buf;
					buf++;
					temp++;
				}
			}
			*temp = *buf;
			temp++;
		}
		buf++;
	}

	if(comma == 1)
	{
	*temp = ',';
	temp++;
	}
	*(temp) = '\0';
}
