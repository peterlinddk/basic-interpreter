#include "variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Variable variables[128];
int variables_used = 0;

static char *type_names[] = {"STRING", "INTEGER", "FLOAT"};

void printVariable(Variable *v);

void variables_dump()
{
  printf("\nVariables:\n");
  for (int i = 0; i < variables_used; i++)
  {
    Variable *v = &variables[i];
    printVariable(v);
  }
}

void printVariable(Variable *v)
{
  printf("%8s - \"%s\": ", type_names[v->type], v->name);
  if (v->type == VAR_TYPE_STRING)
    printf("%s", v->stringValue);
  if (v->type == VAR_TYPE_INTEGER)
    printf("%d", v->intValue);
  if (v->type == VAR_TYPE_FLOAT)
    printf("%f", v->floatValue);

  // TODO: Allow for optional debug to show addresses ...
  // printf(" @%p - value@ %p (%d:%c)", v, &*v->stringValue, *v->stringValue, *v->stringValue);
  printf("\n");
}

Variable *createStringVariable(char *name, char *value)
{
  return createVariable((Variable){.type = VAR_TYPE_STRING, .name = name, .stringValue = value}, name);
}

Variable *createIntVariable(char *name, int value)
{
  return createVariable((Variable){.type = VAR_TYPE_INTEGER, .name = name, .intValue = value}, name);
}

Variable *createFloatVariable(char *name, float value)
{
  return createVariable((Variable){.type = VAR_TYPE_FLOAT, .name = name, .floatValue = value}, name);
}

char *readStringVariable(char *name)
{
  Variable *v = getVariable(name);
  return v != NULL ? v->stringValue : "NULL";
}

int readIntVariable(char *name)
{
  Variable *v = getVariable(name);
  return v != NULL ? v->intValue : 0;
}

float readFloatVariable(char *name)
{
  Variable *v = getVariable(name);
  return v != NULL ? v->floatValue : 0.0;
}

void writeStringVariable(char *name, char *new_value)
{
  Variable *v = getVariable(name);
  if (v != NULL)
  {
    // check type
    if (v->type != VAR_TYPE_STRING)
    {
      printf("TYPE MISMATCH ERROR - expected variable '%s' to be string, got %s\n", name, type_names[v->type]);
      return;
    }
    // free old string
    free(v->stringValue);
    // and set new value
    v->stringValue = strdup(new_value);
  }
}

void writeIntVariable(char *name, int new_value)
{
  Variable *v = getVariable(name);
  if (v != NULL)
  {
    // check type
    if (v->type != VAR_TYPE_INTEGER)
    {
      printf("TYPE MISMATCH ERROR - expected variable '%s' to be integer, got %s\n", name, type_names[v->type]);
      return;
    }
    // set new value
    v->intValue = new_value;
  }
}

void writeFloatVariable(char *name, float new_value)
{
  Variable *v = getVariable(name);
  if (v != NULL)
  {
    // check type
    if (v->type != VAR_TYPE_FLOAT)
    {
      printf("TYPE MISMATCH ERROR - expected variable '%s' to be float, got %s\n", name, type_names[v->type]);
      return;
    }
    // set new value
    v->floatValue = new_value;
  }
}

/**
 * createVariable creates a new variable, inserts it in the list of variables,
 * uses the template for type and value, but gets the name from the parameter
 */
Variable *createVariable(Variable template, char *newname)
{
  // check if variable exists
  Variable *exists = findVariable(newname);
  if (exists != NULL)
  {
    printf("DECLARATION ERROR - variable '%s' already exists!", newname);
    return exists;
  }
  else
  {
    // get a place for the new variable
    Variable *variable = &variables[variables_used++];
    // set the values
    variable->name = strdup(newname);
    variable->type = template.type;
    switch (variable->type)
    {
    case VAR_TYPE_INTEGER:
      variable->intValue = template.intValue;
      break;
    case VAR_TYPE_FLOAT:
      variable->floatValue = template.floatValue;
      break;
    case VAR_TYPE_STRING:
      variable->stringValue = strdup(template.stringValue);
      break;
    }

    return variable;
  }
}

/**
 * getVariable uses findVariable, and sets an error if no variable was found!
 */
Variable *getVariable(char *name)
{
  Variable *v = findVariable(name);
  if (v == NULL)
  {
    printf("VARIABLE ERROR - variable '%s' does not exist!", name);
    // TODO: Handle error
    return NULL;
  }
  return v;
}

/**
 *  findVariable finds a variable by name, or returns NULL
 * */
Variable *findVariable(char *name)
{
  // find variable with this name
  for (int i = 0; i < variables_used; i++)
  {
    Variable *v = &variables[i];
    if (strcmp(v->name, name) == 0)
    {
      // variable is found!
      return v;
    }
  }
  // if no variable exists
  return NULL;
}