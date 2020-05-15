#include "def.h"
#include "def.h"

int seocnd_scan( FILE * fp, char * fileName) {

	char * currLine [MAX_LINE_LENGTH] ;  /* the current line */
	char * argv[MAX_OP_NUM] ; /* holds arguments for command statements */
 	int  erorFlag = 0 ;
	int  lineNumber = 0;
    	int  opFunct, op1Add, op2Add, guideType, argc, current = 0 ;

    	rewind (fp); /* sets the pointer to the begining of the file*/


    	while ( fgets(currLine, MAX_LN_LEN + 1, fp) ) /* stages 1-7 */ 
    	{
		++lineNumber;
		argc = parseCommand(argv, currLine); /* breakes the line into tokens */

        	if ( currLine[0] == ';' )
            		continue; /* skips ';' because it is documention in assembly */

		if ( (labelValid( argv[current] ) >= 0 ) /* stage 2 */
			current++ ; 		
		
		guideType = getGuideType( argv[current] );
        	if  ( guideType == DATA || guideType == STR || guideType == EXTERN  ) /* stage 3 */
            		continue ;

       		if ( guideType == ENTRY )  {   /* stage 4 */ 

            		if ( symbolInTable ( argv[0] ) == NULL )  {
				printf("error in %d: label %s was not found", lineNumber, argv[0] );
               			erorFlag = 1;
                		continue ;
            		}
           		makeEntry ( argv[0] ); /* stage 5 */   
            		continue ;
        	}
		
		op1Add = getAddMthd(argv[current+1]);
		op2Add = getAddMthd(argv[current+2]);
		opFunct = 1; /* suppose to be function that gets the opcode of the instructio */

       		/* stage 6 */
       		buildBinaryCode (opFunct ,argv[current+1],argv[current+2], op1Add,op2Add ) ; 
        
    	}

    	if ( erorFlag )   /* stage 7 */    
        	return -1; /* indicates that eror was found*/

    	buildFiles(fileName); /* stage 8 */
    	return 1; /* indicates to the main that the seocnd scan terminated correctly*/
}
