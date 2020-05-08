#include "machineCode.h"


/* codeInstruction - codes an instruction into the instrcution image
 * INPUT: instrcution name, first operand name, second operand name, and their addressing methods
 * OUTPUT: integer, 0 for good termination
 * METHOD:  complete this later on
 */
int codeInstruction(char* instName, char* srcOper, char* destOper, int sourceAdd, int destAdd) {

	cmdHeader inst; /* the first word aka the istruction code  */
	data  firstWord, secondWord; /* the possible words  */
	line instLine, firstWordLine, secondWordLine ; 
	int flag1 = 0, flag2 = 0 ; /* if first word exsits and if the second word exists  */
	int currentIC = IC; /* the current flace of IC in memory */

	/*coding the first word the instrcution it self */
	inst.opCode = (unsigned int) extractOpCode(instName) ; /* gets all the necessary data for the coding */
	inst.funct = (unsigned int) extractFunct(instName) ;
	inst.srcAdress = (unsigned int) sourceAdd ; 
	inst.destAdress = (unsigned int) destAdd ;
	inst.srcReg = (unsigned int) isRegister(srcOper) ;
	inst.destReg = (unsigned int) isRegister(destOper) ;
	inst.A = 1; /* in the first word of every instruction A = 1 */

	instLine.head = inst;
	instIamge[IC++] = instLine ;/* codes the word and then increment the instruction counter */ 

	/* finding the coding values for the first optional word */
	if ( sourceAdd == IMM ){
		flag1 = 1;
		firstWord.data = extractNum(srcOper);
		firstWord.A = 1 ;
	}	
	
	else if ( sourceAdd == DIR ){
		flag1 = 1;
		firstWord.data = findMemAddress(srcOper);
		if ( isExternal(srcOper) ) 
			firstWord.E = 1 ;
		else /* if it is not external label */
			firstWord.A = 1 ;
	}
	
	else if ( sourceAdd == REL ){
		flag1 = 1;
		firstWord.data = findMemAddress(srcOper) - currentIC ;
		firstWord.A = 1 ;
	} /* in case of register addressing we dont have additional words */	
	
	if (flag1){ /*i f there is a word so it added to the instrucion image */
		firstWordLine.value = firstWord ;
		instIamge[IC++] = firstWordLine ;/* codes the word and then increment the instruction counter */ 
	}		
	

	/* finding the coding values for the second optional word */
	if ( destAdd == IMM ){
		flag2 = 1;
		secondWord.data = extractNum(destOper);
		secondWord.A = 1 ;
	}
		
	else if ( destAdd == DIR ){
		flag2 = 1;
		secondWord.data = findMemAddress(destOper);
		if ( isExternal(destOper) )
			secondWord.E = 1 ;
		else
			secondWord.A = 1 ;
	}
		
	else if ( destAdd == REL){
		flag2 = 1;
		secondWord.data = findMemAddress(destOper) - currentIC;
		secondWord.A = 1 ;
	}
	
	if (flag2){
		secondWordLine.value = secondWord ;
		instIamge[IC++] = secondWordLine ;
	}	


	return 0; /* everything went fine */
}


/* cextractOpCode - extracrts the opCode according to the instruction name
 * INPUT: string
 * OUTPUT: integer,  non negative for good termination, -1 otherwise
 */
static int extractOpCode (char* commandName ){

	int i;

	for( i=0 ; i< INST_NUM ; i++){
		/* if commandName appear in the instructionNames array */
		if ( ! strcmp (commandName, instructionNames[i] ) ) {

			if( i <= 2 || i == 4 || i == 9 || i >= 12) /* replace all  the magic nambers */
				return i; /* in this case opcode = i */

			else if ( i == 3 )
				return 2;

			else if ( i >= 6 && i <= 8)
				return 5;

			else /* i == 10 or i ==11 */
				return 9;
		}
	}

	/* in valid instruction name */
	printf("Error in line %d: instrustion %s is invalid\n", lineNumber,commandName);
	return -1; /* -1 might cause some erors because we are dealling with unsigned integers */
}



/* extractFunct - extracrts the funct according to the instruction name
 * INPUT: string
 * OUTPUT: integer,  non negative for good termination, -1 other wise
 */
static int extractFunct (char* commandName ){

	int i;

	for( i=0 ; i< INST_NUM ; i++){
		/* if commandName appear in the instructionNames array */
		if ( ! strcmp (commandName, instructionNames[i]) ){

			if ( i == 2 || i == 5 || i == 9 ) /* replace all  the magic nambers */
				return 1; 

			else if ( i == 3 || i == 6 || i == 10  )
				return 2;

			else if ( i == 7 || i == 11 )
				return 3;

			else if ( i == 8 )
				return 4;

			else /* no opcode */
				return 0;
		}
	}

	/* in valid instruction name */
	printf("Error in line %d: instrustion %s is invalid\n", lineNumber,commandName);
	return -1; 
}


/* checks if the string is a register name, if it does the function retrun the namber of the register else 0 */
static int isRegister(char *str){

	int regNum;
	char *endChar = NULL; 
	
	if(str[0] == 'r')
	{
		regNum = (int) strtol(str+1, &endChar, 10); /* parse the string into integer */

		if(*endChar != '\0' || endChar == str+1)
			return 0;

		if( regNum>=0 && regNum<=7 )
			return regNum ; /* legit register name */
	}
	return 0; /* not a  register name */
}


/* extractNum- extracrts a number from the stringme
 * INPUT: string
 * OUTPUT: integer, if the string is an integer so it return the number, -1 otherwise
 */
static int extractNum (char *str){

	int num;
	char *endChar = NULL; 

	num = (int) strtol(str, &endChar, 10); /* parse the string into integer */

	if(*endChar != '\0'){
		printf("Error in line %d: extraneous text after end of number in %s \n",lineNumber , str);
		return -1; /* eror code */
	}
	else
		return num ; /* legit number */
}



