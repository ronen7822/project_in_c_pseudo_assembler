#include <stdio.h>
#include <string.h>
#include "firstScan.h"
#include "secondScan.h"
#include "legalTable.h"

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
	/* i'v changed temporary .as to .asb becuse the suffix .as causes the files in my ubuntu to go crazy */
    	if ((fp = fopen(strcat(argv[fileNum], ".asb"), "r")) == NULL) {
    		printf("error in file %s: cannot open file\n", argv[fileNum]);
    		continue;
    	}

    	if ((firstScan(fp) < 0) || (secondScan(fp, argv[fileNum]) < 0))
    		printf("%s: failed to assemble\n", argv[fileNum]);
    	else
    		printf("%s: assemble finished successfully\n", argv[fileNum]);
    	fclose(fp);
    }

    return 0; /* program finished voluntrily */
}
