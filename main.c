#include "def.h"

int check_name ( char * );/* asserts the the file name ends with the suffix ".as" */

int main( int argc, char *argv[] ) 
{
    FILE *fp; /* pointer to the current file  */
    int num_of_files = argc - 1;
    int curr_file = 1; /* index to the current file */
 
    if ( ! num_of_files ) /* no files were given  */
    {
        fprintf( stderr, "\nEROR - there are no given files in the command line\n",); 
        return 0;
    }
   
    do
    {        
        if (  ! check_name( argv[curr_file] ) ) /* asserts the the file name ends with ".as" */
        {
            fprintf( stderr, "EROR - the file %s does not ends with \".as \" \n", argv[curr_file]);
            curr_file++ ;
            continue ; /* continue to the next file */
        }     
        else if( !( fp = fopen( argv[curr_file], "r+") ) ) /* asserts that the file can be found and opended */
        {
            fprintf( stderr, "\nEROR - the file %s can not be found or open\n", argv[curr_file]);
            curr_file++ ;
            continue ; /* continue to the next file */
        } 

        assembler(fp); /* builds the assemblerd files */
        fclose(fp); /* closing the file */
        curr_file++ ;
    }
    while ( curr_file <= argc ) ; /* there is assembly files to read from */
   
    return 1;
}

int check_name ( char * file_name)
{
    int suffix_index = strlen( file_name ) - 4 ;
    const char * suffix = file_name + suffix_index ;

    if ( ! strcmp( suffix, ".as") )
        return 1 ;
    return 0;
}
