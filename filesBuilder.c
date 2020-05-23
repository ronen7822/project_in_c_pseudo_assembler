#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "symbolTable.h"
#include "machineCode.h"

/* the static function */
static int parseLineToNum( line ln ) ;
static int bitTobit (int val);
static int parseData (dataPtr ptr, int type, int offSet, int length, FILE * fpOb) ;
static int printImage( int ICF, dataNode dataImage, FILE * fpOb) ;

/* builds the output files */
int buildFiles(char * fileName, int ICF, dataNode dataImage) {
	
	FILE * fpOb, * fpEnt,* fpExt; /* pointers to the object, entry and extrernal files  */
	char * objectName, * entryName,  * externName ; /* the name of the  creared files */
	symbolNode *ptr = head; /* pointer to the symbol table */
	

	/* creates the object file name*/
	objectName = (char *)malloc(strlen(fileName) + 3); /* 3 is the length of ".as" */
	strcpy(objectName, fileName);
   	strcat(objectName, ASM_OBJECT_SUFF);

	printf("strating to build the files...\n");
	if(  (fpOb = fopen( objectName, "w+")) ) {  /* asserts that the file can be found and opended */

		fprintf(fpOb, "%d %d\n" , ICF-MMRY_OFFSET, DCF); /* the lenght of the instruction image and the data image  */
		printImage( ICF,  dataImage, fpOb ); /* prints the instruction and the data in the object file*/		
		fclose(fpOb);
	}
	else	
		fprintf( stdout, "\nEROR - could not write the object file of %s \n", fileName);


	entryName = (char *) malloc(strlen(fileName) + 3); /* creates the entry file name*/
	strcpy(entryName, fileName);
   	strcat(entryName, ASM_ENTRIES_SUFF);

	if ( findEntryLabel(ptr) && (fpEnt = fopen(entryName, "w+")) ) {
		
		ptr = head; /* the head of the symbolList */
		while( (ptr = findEntryLabel(ptr)) != NULL )   /* search for entry symbols */
			fprintf(fpEnt, "%s %06d\n" ,  ptr->data.name, ptr->data.value ) ;/* prints entry symbols */
		
		fclose(fpEnt);
	}
	else if ( ! findEntryLabel(ptr)  )
		fprintf( stdout, "\nEROR - could not write the entry file of %s \n", fileName);

	
	externName = (char *)malloc( strlen(fileName) + 3); /* creates the extern file name*/
	strcpy(externName, fileName);
   	strcat(externName, ASM_EXTERNALS_SUFF);
	ptr = head; /* the head of the symbolList */

	if ( findExternalLabel(ptr)  && (fpExt = fopen( externName, "w+")) ) {
		ptr = head; /* the head of the symbolList */
		while( (ptr = findExternalLabel(ptr)) != NULL )   /* search for enxternal symbols */
			fprintf(fpExt,"%s %06d \n" , ptr->data.name, ptr->data.value) ; /* prints enxternal symbols */
		
		fclose(fpExt);
	}
	else if ( ! findExternalLabel(ptr)  )
		fprintf( stdout, "\nEROR - could not write the entry file of %s \n", fileName);


	/* free all the malloc's */
	free(objectName);
	free(entryName);
	free(externName);

	return 1;	
}




/* transforms 32 number to his 24 bit equvelent */
static int bitTobit (int val) {

    	int max24 = 0x7FFFFF;  /* limit to 23 bits beacuse thers is the  leading sign bit  */
	int mask = 0x00FFFFFF;

    	if ( val >= 0 && val < max24 ) /* positive number smaller than 2^23   */
       		return val;

   	else if (val < 0 && val >= (-1*max24) )  /* the logic behind this is to trasfer the 8 leading bits to zero's */
        	return (val&mask) ; 

	else {
		printf("warning in %d: the number :%d is cannot be written in 24 bits format\n", lineNumber, val);
		return -1;
	}
}

/* parse line to integer */	
static int parseLineToNum( line ln ) {

	int num = 0;

	if ( ln.type == HEADER_LINE ) {
		num += ln.conent.head.E ;
		num += ln.conent.head.R<<1 ;
		num += ln.conent.head.A<<2 ;
		num += ln.conent.head.funct<<3 ;
		num += ln.conent.head.destReg<<8 ;
		num += ln.conent.head.destAdress<<11 ;
		num += ln.conent.head.srcReg<<13 ;
		num += ln.conent.head.srcAdress<<16;
		num += ln.conent.head.opCode<<18;
	}

	else if (ln.type == DATA_LINE) { 
		num += ln.conent.value.E ;
    		num += ln.conent.value.R<<1 ;
   		num += ln.conent.value.A<<2 ;
   		num += ln.conent.value.data<<3 ;
	}

	/* transforms 32 number to his 24 bit equvelent */
	return bitTobit (num);
} 



/*parse dataNode which is string or integers into machine code */
static int parseData (dataPtr ptr, int type, int offSet, int length, FILE * fpOb) {

	int  i; /* the length of the array */	
	
	if (type == DATA) { /* integers */		
		for (i=0 ; i < length ; i++  ) 
			fprintf (fpOb, "%06d %06x\n" , 100+offSet+i , bitTobit ( ptr.intPtr[i] ) ) ;
	}
	
	else if (type == STR) { /* string  */		
		for (i=0 ; i < length ; i++  ) 
			fprintf (fpOb, "%06d %06x\n" , 100+offSet+i , ptr.strPtr[i] ) ;
	}
	return length;
}


static int printImage( int ICF, dataNode dataImage , FILE * fpOb) {

	int i ;
	dataNode* dataImagePtr = &dataImage;
	
	for (i=0 ; i < ICF-100 ; i++ ) 
		fprintf(fpOb, "%06d %06x\n" , i+100, parseLineToNum (instIamge[i]) ) ; 

	while( dataImagePtr ) {
		i += parseData (dataImagePtr->data, dataImagePtr->type , i ,dataImagePtr->length, fpOb  ) ;
		dataImagePtr = dataImagePtr->next;
	}
	

	return 1;
}

