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
	{   
      		token = strtok(NULL, s); /* gets the n'th word in the sentence  */
        }
        return token
}


static int isGuidance(char *str)
{
	int i;

	for (i=0; i<2 ; i++)
	{
		if( ! strcmp(guidanceTypes[i], str) ) /* if equal to ".data" or ".string" */
		{
			return i+1 ;
		}
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
		{
			return i+1 ;
		}
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
		{
			return 0;
		}
		if( regNum>=0 && regNum<=7 )
		{
			return regNum;
		}
	}
	return 0;
}

int isLegitLabelName(char *str)
{
	char temp[MAX_LABEL_SIZE];
	int i=0;
	if(!isalpha(*str))
	{
		printf("Error in line %d: Label name starts with a digit\n", lineNumber);
		return 0;
	}
	while(*str != ':' && *str != '\0')
	{
		if(i>30)
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
	if(isRegister(temp)>=0 || isInstruction(temp)>=0)
	{
		printf("Error in line %d: Label name cannot be register or instruction name\n", lineNumber);
		return 0;
	}
	return i;
}

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
		if(tempNum >= 512 || tempNum< -512)
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
			if(*(str+1) == '\0')
			{
				printf("Error in line %d: Comma is dispositioned\n", lineNumber);
				return 0;
			}
			if(*(str+1) == ',')
			{
				printf("Error in line %d: Multiple consecutive commas\n", lineNumber);
				return 0;
			}
		}
		str++;
	}
	return 1;
}
