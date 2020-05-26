/* secondScan - "scan" the file in the second time. in practice, doesn't scan the file but the data structures from the first scan
 * mark entry labels as entry, resolve symbols in file image from symbol table
 * save the lines that has external reference for extern file
 * INPUT: void (data structure saved between first and second scan, and the scan use only their abstractions
 * OUTPUT: status code - 0 if success, ERROR (-1) if an error encountered
 * METHOD: based on the algorithm in the book, but no re-parsing of the file. the similar step is written in comment for each part of the code
 */
int secondScan(void);
