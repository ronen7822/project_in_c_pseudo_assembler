#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "legalTable.h"
#include "symbolTable.h"
#include "firstScan.h"
#include "secondScan.h"
#include "buildFiles.h"

#define FILE_AS(fileName) strcat((fileName), ".as") /* add .as suffix to file name */

/* main - main function of the program
 * INPUT: argv - list of files to assemble
 * OUTPUT: return code - 0 for voluntary exit (although if file assembly was failed because error was encountered), 1 for fatal error
 * METHOD: for each file, run first scan, second scan and build the needed files
 */
int main(int argc, char *argv[])
{
    FILE *fp; /* pointer to the current file  */
    int fileNum = 1; /* index to the current file */
    char *fileName;
 
    if (argc == 1) /* no files were given  */
        printf("warning: no files to assemble\n");

    if (initLegalTable() < 0)
    	return 1; /* cannot access to legal table - cannot assemble the file. fatal error */

    for (fileNum = 1; fileNum < argc; fileNum++) {

    	/* copy file name into independent char, because file name should be manipulated */
    	fileName = calloc((strlen(argv[fileNum]) + 4), sizeof(char));
    	strcpy(fileName, argv[fileNum]);

    	/* print head for each file - make it easier to identify between the files */
    	printf("================== %15s ==================\n", FILE_AS(fileName));

    	/* open file, print error if fails */
    	if ((fp = fopen(fileName, "r")) == NULL) {
    		printf("error in file %s: cannot open file\n", fileName);
    		free(fileName);
    		continue;
    	}

    	/* scan the file to create its image */
    	if ((firstScan(fp) < 0) || (secondScan() < 0)) { /* failure */
    		printf("%s: failed to assemble\n", fileName);

    		/* failure: free all memory allocations */
    		freeDataImage();
    		freeSymbolTable();
    		freeEntryList();
    	}

    	else {
    		DELETE_SUFFIX(3); /* delete the suffix ".as" */

    		/* build the output files */
    		buildImageFile(fileName);

    		if (needEntFile()) /* if there are entries statements, create entries file */
    			buildEntFile(fileName);

    		if (needExtFile()) /* if there are extern statements, create externs file */
    			buildExtFile(fileName);

    		printf("%s: assemble finished successfully\n", FILE_AS(fileName));
    	}
    	free(fileName);
    	fclose(fp);
    }

    return 0; /* program finished voluntrily */
}
