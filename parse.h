#include "def.h"

char *getLabel(char *line);

char *getAnotherLabel(char *line);

int labelValid(char *label);

int getGuideType(char* line);

int getAddMthd(char* op);

dataNode *getNumbers(char* line);

dataNode *getString(char* line);

int parseCommand(char *argv[MAX_OP_NUM], char *cmd);


