#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"
#include "symbolTable.h"
#include "machineCode.h"
#include "buildFiles.h"

#define MASK 0xFFFFFF /* mask to zero spare bits (for total length of 24 bits at most) */

/* buildImageFile - build the image file (contains the instruction and data image)
 * INPUT: file name
 * OUTPUT: void, but build the image file (.ob)
 * METHOD: first print the coded instuction lines to the file, then print the data image to file using appropriate functions
 */
void buildImageFile(char *fileName) {
	int IC, i;
	dataNode *dataPtr = dataImage;

	FILE *fp = fopen(strcat(fileName, ".ob"), "w");
	DELETE_SUFFIX(3); /* delete the suffix ".ob" */

	/* print instruction image */
	fprintf(fp, "%7d %d\n", ICF, DCF);
	for (IC = 0; IC < ICF; IC++)
		fprintf(fp, "%07d %06x\n", IC + MMRY_OFFSET, (lineForPrint(IC) & MASK));

	/* print data image */
	while (dataPtr != NULL) {
		if (dataPtr->type == DATA) {
			for (i = 0; i < dataPtr->length; i++)
				fprintf(fp, "%07d %06x\n", (IC++) + MMRY_OFFSET, (dataPtr->data.intPtr[i] & MASK));
		}
		else {
			for (i = 0; i < dataPtr->length; i++)
				fprintf(fp, "%07d %06x\n", (IC++) + MMRY_OFFSET, (dataPtr->data.strPtr[i] & MASK));
		}
		dataPtr = dataPtr->next;
	}

	freeDataImage(); /* free the memory allocations of the data image */

	fclose(fp);
}

/* buildEntFile - build the entry file (as defined in the exercise)
 * INPUT: file name
 * OUTPUT: void, but build the entry file (.ent)
 * METHOD: scan the symbol table and print all the entry symbols
 */
void buildEntFile(char *fileName) {
	FILE *fp = fopen(strcat(fileName, ".ent"), "w");
	DELETE_SUFFIX(4); /* delete the suffix ".ent" */

	printEntry(fp);
	freeSymbolTable();
	fclose(fp);
}

/* buildExtFile - build the extern file (as defined in the exercise)
 * INPUT: file name
 * OUTPUT: void, but build the extern file (.ext)
 * METHOD: scan the extern list and print all its records
 */
void buildExtFile(char *fileName) {
	FILE *fp = fopen(strcat(fileName, ".ext"), "w");
	DELETE_SUFFIX(4); /* delete the suffix ".ext" */

	printExtern(fp);
	fclose(fp);
}

/* freeDataImage - free the data image
 * INPUT: void (data image is an extern variable)
 * OUTPUT: void, but free all the memory allocation (memory allocations for data and for the node)
 * *this function is in this file because there is no appropriate file for it and this file use it
 */
void freeDataImage() {
	dataNode *dataPtr = dataImage, *forFree;

	while (dataPtr != NULL) {
		/* free allocation for string or numbers */
		free(dataPtr->data.intPtr);

		/* free the node itself */
		forFree = dataPtr;
		dataPtr = dataPtr->next;
		free(forFree);
	}
}
