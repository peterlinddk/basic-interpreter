#include "variables.h"
#include <stdio.h>
#include <string.h>

variable variables[128];
int variables_used = 0;

void variables_dump()
{
  for (int i = 0; i < variables_used; i++)
  {
    variable *v = &variables[i];
    printf("Variable #%d @%p - has name: \"%s\" and value: _%s_\n", i, v, v->name, v->value);
    printf(" - name@%p and value@%p\n", v->name, v->value);
  }
}

void variable_create(char *name, char *value)
{
  // create new struct, and add to the list
  variable v = {.name = strdup(name), .value = strdup(value)};
  // NOTE: I have a feeling that I am creating one struct here, and then copying to another inside the array ...
  printf("variable created at address: %p\n", &variables[variables_used]);
  variables[variables_used++] = v;
}

void variable_write(char *name, char *newvalue)
{
  // search through variables for one with 'name'
  for (int i = 0; i < variables_used; i++)
  {
    variable *v = &variables[i];
    if (strcmp(v->name, name) == 0)
    {
      printf("Variable named %s found!\n", name);
      // free the old value from this variable, to avoid leaks
      // free(v->value);
      v->value = strdup(newvalue);
      break;
    }
  }
  // dump the variables for debugging ...
  variables_dump();
}

char *variable_read(char *name)
{
  // search through variables for one with 'name'
  for (int i = 0; i < variables_used; i++)
  {
    variable *v = &variables[i];
    if (strcmp(v->name, name) == 0)
    {
      // variable is found!
      return v->value;
    }
  }
  return "null";
}
