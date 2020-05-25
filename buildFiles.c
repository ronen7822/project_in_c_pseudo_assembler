#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"
#include "symbolTable.h"
#include "machineCode.h"

#define MASK 0xFFFFFF /* mask to zero spare bits (for total length of 24 bits at most */

int buildImageFile(char *fileName) {
	int IC, i;
	dataNode *dataPtr = &dataImage, *forFree;

	FILE *fp = fopen(strcat(fileName, ".ob"), "w");
	fileName[strlen(fileName) - 3] = '\0'; /* delete the suffix ".ob" */

	/* print instruction image */
	fprintf(fp, "%7d %d\n", ICF, DCF);
	for (IC = 0; IC < ICF; IC++)
		fprintf(fp, "%07d %06x\n", IC + MMRY_OFFSET, (lineForPrint(IC) & MASK));

	/* print data image */
	while (dataPtr != NULL) {
		if (dataPtr->type == DATA) {
			for (i = 0; i < dataPtr->length; i++)
				fprintf(fp, "%07d %06x\n", (IC++) + MMRY_OFFSET, (dataPtr->data.intPtr[i] & MASK));
			/* free(dataPtr->data); */
		}
		else {
			for (i = 0; i < dataPtr->length; i++)
				fprintf(fp, "%07d %06x\n", (IC++) + MMRY_OFFSET, (dataPtr->data.strPtr[i] & MASK));
			/* free(dataPtr->data.strPtr); */
		}

		/* free the data image (no need anymore) move to next node */
		forFree = dataPtr;
		dataPtr = dataPtr->next;
		/* free(forFree); */
	}

	fclose(fp);
	return 0;
}

void buildEntFile(char *fileName) {
	FILE *fp = fopen(strcat(fileName, ".ent"), "w");
	fileName[strlen(fileName) - 4] = '\0'; /* delete the suffix ".ent" */

	printEntry(fp);
	fclose(fp);
}

void buildExtFile(char *fileName) {
	FILE *fp = fopen(strcat(fileName, ".ext"), "w");
	fileName[strlen(fileName) - 4] = '\0'; /* delete the suffix ".ext" */

	printExtern(fp);
	fclose(fp);
}


