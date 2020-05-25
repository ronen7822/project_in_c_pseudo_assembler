#include <stdio.h>
#include <string.h>
#include "legalTable.h"
#include "symbolTable.h"
#include "firstScan.h"
#include "secondScan.h"
#include "buildFiles.h"

int main(int argc, char *argv[])
{
    FILE *fp; /* pointer to the current file  */
    int fileNum = 1; /* index to the current file */
 
    if (argc == 1) /* no files were given  */
        printf("warning: no files to assemble\n");

    if (initLegalTable() < 0)
    	return 1; /* cannot access to legal table - cannot assemble the file. fatal error */

    for (fileNum = 1; fileNum < argc; fileNum++) {

    	/* open file, print error if fails */
    	if ((fp = fopen(strcat(argv[fileNum], ".as"), "r")) == NULL) {
    		printf("error in file %s: cannot open file\n", argv[fileNum]);
    		continue;
    	}

    	/* scan the file to create its image */
    	if ((firstScan(fp) < 0) || (secondScan() < 0)) /* failure */
    		printf("%s: failed to assemble\n", argv[fileNum]);

    	else {
    		(argv[fileNum])[strlen(argv[fileNum]) - 3] = '\0'; /* delete the suffix ".as" */

    		/* build the output files */
    		buildImageFile(argv[fileNum]);

    		if (needEntFile()) /* if there are entries statements, create entries file */
    			buildEntFile(argv[fileNum]);

    		if (needExtFile()) /* if there are extern statements, create externs file */
    			buildExtFile(argv[fileNum]);

    		printf("%s: assemble finished successfully\n", argv[fileNum]);
    	}
    	fclose(fp);
    }

    return 0; /* program finished voluntrily */
}
