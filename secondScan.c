#include "def.h"
#define max_operands 2

int seocnd_scan( FILE * fp)
{
    int i ;
    char * curr_line [MAX_LINE_LENGTH];  /* the current line */
    int eror_flag = 0 ;
    int current = 0; /* the word in the sentence*/


    rewind (fp); /* sets the pointer to the begining of the file*/

    while ( fscanf(fp, "%s", curr_line) ) /* stages 1-7 */ 
    {
        if ( curr_line[0] == ';' )
             continue; /* skips ';' because it is documention in assembly */
        if ( is_symbol( curr_line, current ) ) /* stage 2 */
             current ++ ; 
        if  ( data_storage( curr_line, current ) ||  is_extern(curr_line, current) ) /* stage 3 */
             continue ;

        if ( is_entry(curr_line, current) ) /* stage 4 */
        {
            if (! findLabel ( is_symbol( curr_line, current) ) ) 
            {
                eror_flag = 1;
                continue ;
            }
            make_entry ( is_symbol( curr_line, current) ); /* stage 5 */   
            continue ;
        }

        for (i=current; i++; i< max_operands) /* stage 6- complete this stage */
        {
             if ( is_symbol( curr_line, i) )
             {
                  if (! findLabel ( is_symbol( curr_line, i) ) ) 
            		eror_flag = 1;
                  else
                  {
 			if ( is_extern(curr_line,i) )
                             ; /* add to dedicated external file */

                        build_word( findLabel ( is_symbol( curr_line, i) ) ); /* builds the word in maching code */
                  }
             }
             continue;
        }
        
    }
    /* stage 7 */
    if ( eror_flag )       
        return 0; /* indicates that eror was found*/

    build_output_files(fp); /* stage 8 */
    return 1; /* indicates to the main that the seocnd scan terminated correctly*/
}


