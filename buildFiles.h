/* buildImageFile - build the image file (contains the instruction and data image)
 * INPUT: file name
 * OUTPUT: void, but build the image file (.ob)
 * METHOD: first print the coded instuction lines to the file, then print the data image to file using appropriate functions
 */
void buildImageFile(char *fileName);

/* buildEntFile - build the entry file (as defined in the exercise)
 * INPUT: file name
 * OUTPUT: void, but build the entry file (.ent)
 * METHOD: scan the symbol table and print all the entry symbols
 */
void buildEntFile(char *fileName);

/* buildExtFile - build the extern file (as defined in the exercise)
 * INPUT: file name
 * OUTPUT: void, but build the extern file (.ext)
 * METHOD: scan the extern list and print all its records
 */
void buildExtFile(char *fileName);

/* freeDataImage - free the data image
 * INPUT: void (data image is an extern variable)
 * OUTPUT: void, but free all the memory allocation (memory allocations for data and for the node)
 * *this function is in this file because there is no appropriate file for it and this file use it
 */
void freeDataImage(void);
