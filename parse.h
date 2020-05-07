#include "def.h"

char *getLabel(char *line);

char *getAnotherLabel(char *line);

int labelValid(char *label);

int getGuideType(char* line);

int getAddMthd(char* op);

int getNumbers(char* line, dataContent array);

int getString(char* line, dataContent array);

int parseCommand(char *argv[MAX_OP_NUM], char *cmd)

static int addArg(char* cmd, char *argv[MAX_OP_NUM ], int i, int argStart, int argc);
  
static void flushArgv(char *argv[MAX_OP_NUM]);
