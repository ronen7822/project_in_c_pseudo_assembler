#include "def.h"
#include "symbolTable.c"

/* builds the output files */
int buildFiles(char * fileName) {
	
	FILE * fpOb, fpEnt, fpExt; /* pointers to the object, entry and extrernal files  */
	char * objectName, entryName, externNAame ; /* the name of the  creared files */
	symbolNode *ptr = head; /* pointer to the symbol table */
	int i;
	

	/* creates the object file name*/
	objectName = (char *)malloc(strlen(fileName) + 3); /* 3 is the length of ".as" */
	strcpy(objectName, fileName);
   	strcat(objectName, ASM_OBJECT_SUFF);

	if(  (fpOb = fopen( objectName, "w+")) ) {  /* asserts that the file can be found and opended */

		fprintf(fpOb, "%d %d\n" , ICF-MMRY_OFFSET, IDF); /* the lenght of the instruction image and the data image  */

		for (i=0 ; i < ICF-MMRY_OFFSET ; i++ )  /* prints the instructiom image */
			fprintf(fpOb, "%06d %06x\n" , i+MMRY_OFFSET  , parseLineToNum (instIamge[i]))  ;
		
		for (i=0 ; i < IDF ; i++ )  /* prints the data image */
			fprintf(fpOb, "%06d %06x\n" , i+ICF ,parseDataLineToNum (dataImage[i])) ;
	}
	else	
		fprintf( stdout, "\nEROR - could not write the object file of %s \n", fileName);



	entryName = (char *) malloc(strlen(fileName) + 3); /* creates the entry file name*/
	strcpy(entryName, fileName);
   	strcat(entryName, ASM_ENTRIES_SUFF);

	if ( findEntryLabel() >= 0 && (fpEnt = fopen(entryName, "w+")) ) {

		while( ptr = findEntryLabel(ptr) )   /* search for entry symbols */
			fprintf(fpEnt, "%s %06d\n" ,  ptr->data.name, ptr->data.value ) ;/* prints entry symbols */
	}
	else if ( findEntryLabel() >= 0 )
		fprintf( stdout, "\nEROR - could not write the entry file of %s \n", fileName);

	

	externName = (char *)malloc(strlen(fileName) + 3); /* creates the extern file name*/
	strcpy(externName, fileName);
   	strcat(externName, ASM_EXTERNALS_SUFF);

	if ( findExternalLabel() >= 0 && (fpExt = fopen( externNAame, "w+")) ) {

		ptr = head; /* the head of the symbolList */
		while( ptr = findExternalLabel(ptr) )   /* search for enxternal symbols */
			fprintf(fpExt,"%s %06d \n" , ptr->data.name, ptr->data.value) ;/* prints enxternal symbols */
	}
	else if ( findExternalLabel() >= 0 )
		fprintf( stdout, "\nEROR - could not write the entry file of %s \n", fileName);


	/* free all the malloc's */
	free(objectName);
	free(entryName);
	free(externName);

	return 1;	
}

/* parse line to integer */
int parseLineToNum( line ln ) {

	int num = 0;

	num += ln.head.E ;
	num += ln.head.R<<1 ;
	num += ln.head.A<<2 ;
	num += ln.head.funct<<3 ;
	num += ln.head.destReg<<8 ;
	num += ln.head.destAdress<<11 ;
	num += ln.head.srcReg<<13 ;
	num += ln.head.srcAdress<<16;
	num += ln.head.opCode<<18;

	return num;
}

/* parse data line to integer */
int parseDataLineToNum( line ln ) {

    int num = 0;
    /* shift bits is equal to multipling by pow(2,i) */
    num += ln.value.E ;
    num += ln.value.R<<1 ;
    num += ln.value.A<<2 ;
    num += ln.value.data<<3 ;

    return num;
}
