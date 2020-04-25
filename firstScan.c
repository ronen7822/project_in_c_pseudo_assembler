#include "def.h"
#include "legalTable.h"
#include "parsing.h"

/* the first scan in the dual scan algorithm, no header file for now, could be added in the future */
int first_scan( FILE * fp)
{
    char  curr_line [MAX_LINE_LENGTH]; /* the current line */
    char label_name [MAX_LINE_LENGTH] ;
    int i; 
    int num_of_bytes ;
    int eror_flag = 0; /* indicates if eror was found */
    int symbol_flag =0; /* indicates if symbol was counterd */
    extern int DCF; /* data counter final */
    extern int ICF; /* instructiom counter final */
    DC = 0;
    IC = MMRY_OFFSET;
  
    rewind (fp); /* sets the pointer to the begining of the file*/
  
    /* the unknown methods shoud be defined the other files */
    while ( fscanf(fp, "%s", curr_line) ) /* stages 2-16 */ 
    {
        if ( curr_line[0] == ';' )
             continue; /* skips ';' because it is documention in assembly */

        if ( is_symbol( curr_line , 0) ) /* stage 3 */
             symbol_flag = 1; /* symbol was counterd  - stage 4 */

 	if ( is_guidance( curr_line, 1 ) ) /*  if it is guidance line - stage 5 */
        {
             if ( symbol_flag ) /* stages 6-7 in the algorithm */
             {
                  /* adds the lable to the symbol table*/
                  label_name  = extract_word( curr_line, 0 ) ; /* gets the name of the label */
                  if( ! addLabelToList ( label_name, DATA, DC) )
                         eror_flag = 1; /* the lable already exist in symbol table*/
             }
             /* puts the data in the data image - stage 7 */
             storeData( curr_line )  ;            
         }
 
         if ( is_extern(curr_line, 0)  || is_entry(curr_line, 0) ) /* stage 8 */
         {
             if ( is_entry(curr_line, 0) ) /* stage 9 */
                 continue; /* will be taken care of in the second scan */
             else /* is external.  stage 10 */
             {
                  label_name [] = extract_word( curr_line, 1 ) ; /* gets the name of the external label */
                  addLabelToList( label_name, EXTERNAL, 0) ;
             }
         }
        
         /* it is command line stges 11-16 */
         if ( symbol_flag ) 
         {
               /* adds the lable to the symbol table*/
               label_name  = extract_word( curr_line, 0) ; 

               if( ! addLabelToList (label_name, CODE, IC) )
                      eror_flag = 1; 
         }
        
         if ( ! search_oper_in_legalTable( curr_line ) ) /* stage 12 */ 
                eror_flag = 1; /* operation does not exist*/

         num_of_bytes = analyze_operation( curr_line ) ; /* stage 13 */
       
         build_word(curr_line , 0 ) ; /* stage 14 */
         for (i=1; i++; i<num_of_bytes) 
         {
              if ( immidiate_addressing(curr_line, i) )
                   build_word(curr_line , i); /* builds the word in maching code */
         }
     
         save_counters(curr_line, num_of_bytes, IC); /* stage 15 */
      
         IC = IC + num_of_bytes ; /* stage 16 */
         symbol_flag = 0;
    }

    /* eror_flag is set to 1 if eror was found in the firs scan, stage 17 */
    if ( eror_flag )       
        return 0; /* indicates that eror was found*/
     
    DCF = DC; /* stage 18 */
    ICF = IC;

    updateDataLabels( ICF ) ; /* stage 19 */

    return 1 ; /* indicates to the main method to start the second scan. stage 20 */ 
}

