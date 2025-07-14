#ifndef VARIABLES_H
#define VARIABLES_H

typedef struct
{
  char *name;
  char *value;
} variable;

void variables_dump();
void variable_create(char *name, char *value);
void variable_write(char *name, char *newvalue);
char *variable_read(char *name);

#endif