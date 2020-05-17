#include <stdio.h>
#include <string.h>
#include "firstScan.h"
#include "secondScan.h"

int checkName (char *fileName);/* asserts the the file name ends with the suffix ".as" */

int main( int argc, char *argv[] ) 
{
    FILE *fp; /* pointer to the current file  */
    int fileNum = 1; /* index to the current file */
 
    if (argc == 1) /* no files were given  */
        printf("warning: no files to assemble\n");

    for (fileNum = 1; fileNum < argc; fileNum++) {
    	/* check file name - need to have the correct suffix */
    	if (!checkName(argv[fileNum])) {
    		printf("error in file %s: invalid file name\n", argv[fileNum]);
    		continue;
    	}

    	/* open file, print error if fails */
    	if ((fp = fopen(argv[fileNum], "r")) == NULL) {
    		printf("error in file %s: cannot open file\n", argv[fileNum]);
    		continue;
    	}

    	if ((firstScan(fp) < 0) || (secondScan(fp) < 0))
    		printf("%s: failed to assemble\n", argv[fileNum]);
    	else
    		printf("%s: assenble finished successfully\n", argv[fileNum]);
    	fclose(fp);
    }

    return 0; /* program finished voluntrily */
}

int checkName (char *fileName) {
    int suffixIndex = strlen( fileName ) - 4 ;
    const char * suffix = fileName + suffixIndex ;

    if (!strcmp(suffix, ".as"))
        return 1;
    return 0;
}
