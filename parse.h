#include "def.h"

char *getLabel(char *line);

char *getAnotherLabel(char *line);

int labelValid(char *label);

int getGuideType(char* line);

int getAddMthd(char* op);

int parseCommand(char *argv[MAX_OP_NUM], char* line);

int getNumbers(char* line);

int getString(char* line,  dataContent array);
