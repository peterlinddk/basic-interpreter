#ifndef VARIABLES_H
#define VARIABLES_H

typedef enum
{
  VAR_TYPE_STRING,
  VAR_TYPE_INTEGER,
  VAR_TYPE_FLOAT
} VariableType;

typedef struct
{
  char *name;
  VariableType type;
  union
  {
    char *stringValue;
    int intValue;
    float floatValue;
  };
} Variable;

void variables_dump();

Variable *createStringVariable(char *name, char *value);
Variable *createIntVariable(char *name, int value);
Variable *createFloatVariable(char *name, float value);
char *readStringVariable(char *name);
int readIntVariable(char *name);
float readFloatVariable(char *name);
void writeStringVariable(char *name, char *new_value);
void writeIntVariable(char *name, int new_value);
void writeFloatVariable(char *name, float new_value);

Variable *createVariable(Variable template, char *newname);
Variable *getVariable(char *name);
Variable *findVariable(char *name);

#endif