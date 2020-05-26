/* firstScan - scan the file for the first time, build the file image (labels are not resolved in this scan, done in the second scan)
 * entry statement saved in entry list, and resolved in second scan
 * build data image - save the values in linked list (second scan print them in file)
 * INPUT: a file to scan
 * OUPUT: status code - 0 in success, ERROR (-1) if an error encountered
 * METHOD: the algorithm based on the algorithm from the book, but with changes to make it more efficient.
 * the appropriate stage number is noted before the code
 */
int firstScan(FILE *fp);
