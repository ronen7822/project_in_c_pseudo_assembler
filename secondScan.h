#include "def.h"


typedef struct externNode {
	struct externNode *next;
	char name[MAX_LBL_SZ + 1];
	int lineVal;
} externNode;

int buildFiles(char * fileName, int ICF, dataNode dataImage);

int addExternNode(int lineNum, char *symbol) ;

int secondScan(FILE *fp , char * fileName);

externNode * findExternalLabel(externNode *ptr);


extern externNode *headExt ;
