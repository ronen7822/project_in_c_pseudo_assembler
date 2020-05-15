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
			fprintf(fpOb, "%06d %06x\n" , i+MMRY_OFFSET  , instIamge[i]) ;
		
		for (i=0 ; i < IDF ; i++ )  /* prints the data image */
			fprintf(fpOb, "%06d %06x\n" , i+ICF, dataImage[i]) ;
	}
	else	
		fprintf( stdout, "\nEROR - could not write the object file of %s \n", fileName);



	entryName = (char *) malloc(strlen(fileName) + 3); /* creates the entry file name*/
	strcpy(entryName, fileName);
   	strcat(entryName, ASM_ENTRIES_SUFF);

	if ( findEntryLabel(head) >= 0 && (fpEnt = fopen(entryName, "w+")) ) {
		ptr = head;
		while( ptr = findEntryLabel(ptr) )   /* search for entry symbols */
			fprintf(fpEnt, "%s %06d\n" ,  ptr->data.name, ptr->data.value ) ;/* prints entry symbols */
	}
	else if ( findEntryLabel(head) >= 0 )
		fprintf( stdout, "\nEROR - could not write the entry file of %s \n", fileName);

	

	externName = (char *)malloc(strlen(fileName) + 3); /* creates the extern file name*/
	strcpy(externName, fileName);
   	strcat(externName, ASM_EXTERNALS_SUFF);

	if ( findExternalLabel(head) >= 0 && (fpExt = fopen( externNAame, "w+")) ) {
		ptr = head; /* the head of the symbolList */
		while( ptr = findExternalLabel(ptr) )   /* search for enxternal symbols */
			fprintf(fpExt,"%s %06d \n" , ptr->data.name, ptr->data.value) ;/* prints enxternal symbols */
	}
	else if ( findExternalLabel(head) >= 0 )
		fprintf( stdout, "\nEROR - could not write the entry file of %s \n", fileName);


	/* free all the malloc's */
	free(objectName);
	free(entryName);
	free(externName);

	return 1;	
}


