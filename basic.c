#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int startsWithIgnoreCase(char *string, const char *word);
char escapeChar(char c);

void variables_dump();
void variable_create(char *name, char *value);
void variable_write(char *name, char *newvalue);
char *variable_read(char *name);

void kw_let(char *parameter);
void kw_print(char *parameter);
void kw_input(char *parameter);

const char *keywords[] = {"NEW", "LIST", "RUN", "END", "REM", "LET", "PRINT", "INPUT"};

int main()
{
  printf("Welcome to BASIC\n");
  char inputline[255] = "";

  do
  {
    printf("ready.\n");
    printf(">");

    fgets(inputline, 255, stdin);
    // remove last newline
    inputline[strlen(inputline) - 1] = '\0';

    // check if the inputstring contains a keyword
    char *inp = inputline;

    // ignore leading whitespace
    while (*inp == ' ' || *inp == '\t')
      inp++;

    // compare input with each keyword in list
    for (int kw = 0; kw < 8; kw++)
    {
      if (startsWithIgnoreCase(inp, keywords[kw]))
      {
        printf("Recognized keyword: %s in %s\n", keywords[kw], inp);
        switch (kw)
        {
        case 5:
          kw_let(inp + strlen(keywords[kw]));
          break;
        case 6:
          kw_print(inp + strlen(keywords[kw]));
          break;
        case 7:
          kw_input(inp + strlen(keywords[kw]));

        default:
          break;
        }

        // don't look at any more keywords
        break;
      }
    }

    //    printf("You entered '%s'\n", inputline);
  } while (!startsWithIgnoreCase(inputline, "QUIT"));

  return 0;
}

int startsWithIgnoreCase(char *string, const char *word)
{
  while (*word)
  {
    if (toupper(*string++) != *word++)
    {
      return 0;
    }
  }
  return 1;
}

typedef struct
{
  char *name;
  char *value;
} variable;

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

void kw_let(char *parm)
{
  printf("LET %s\n", parm);

  // variable name and value
  char name[255] = "";
  char *nam = name;
  char value[255] = "";
  char *val = value;

  // ignore whitespace in parameter
  while (*parm == ' ' || *parm == '\t')
    parm++;
  // put everything before the = into name
  while (*parm != '=')
  {
    *nam++ = *parm++;
  }
  // remove trailing whitespace from name
  while (*(nam - 1) == ' ' || *(nam - 1) == '\t')
  {
    nam--;
  }
  *nam = '\0';

  parm++;
  // remove leading whitespace in value
  while (*parm == ' ' || *parm == '\t')
    parm++;
  // and put everything after = into value
  while (*parm != '\0')
  {
    *val++ = *parm++;
  }
  // but remove trailing whitespace from value
  while (*(val - 1) == ' ' || *(val - 1) == '\t')
  {
    val--;
  }
  *val = '\0';

  // print variable name and value
  //  printf("LET variable: _%s_ be value: _%s_\n", name, value);
  // create variable
  variable_create(name, value);
  variables_dump();
}

void kw_print(char *parm)
{
  // ignore whitespace in parameter
  while (*parm == ' ' || *parm == '\t')
    parm++;

  printf("PRINT %s\n", parm);

  // create a new string to print
  char string[255] = "";
  char *str = string;

  // find start of string in input
  char *input = parm;
  if (*input != '"')
  {
    // it is a variable name!
    char name[255] = "";
    char *nam = name;
    while (*input != '\0')
    {
      *nam++ = *input++;
    }
    // remove trailing whitespace, if any
    while (*(nam - 1) == ' ' || *(nam - 1) == '\t')
    {
      nam--;
    }
    *nam = '\0';

    //    printf("PRINT variable named '%s': _%s_\n", name, variable_read(name));
    char *value = variable_read(name);
    while (*value != '\0')
      *str++ = *value++;
  }
  else
  {
    // it is a string
    input++; // ignore the first "

    // add each character to the string
    while (*input != '"')
    {
      if (*input != '\\')
      {
        *str++ = *input++;
      }
      else
      {
        // character was \ - read next character
        input++;
        *str++ = escapeChar(*input++);
      }
    }
    // make sure to terminate the string
    *str = '\0';
  }

  // print string
  printf("%s", string);
}

void kw_input(char *parm)
{
  printf("INPUT %s\n", parm);

  // ignore leading whitespace in parameter
  while (*parm == ' ' || *parm == '\t')
    parm++;
  char *input = parm;
  // check if we begin with a string
  if (*input == '"')
  {
    // create string
    char string[255] = "";
    char *str = string;

    input++; // ignore the first "

    // add each character to the string
    while (*input != '"')
    {
      if (*input != '\\')
      {
        *str++ = *input++;
      }
      else
      {
        // character was \ - read next character
        input++;
        *str++ = escapeChar(*input++);
      }
    }
    // make sure to terminate the string
    *str = '\0';
    // and scroll past the last "
    input++;

    // print the prompt string
    printf("%s", string);
  }

  // find variable name - skip leading whitespace
  while (*input == ' ' || *input == '\t')
    input++;

  char name[255] = "";
  char *nam = name;
  while (*input != '\0')
  {
    *nam++ = *input++;
  }
  // remove trailing whitespace, if any
  while (*(nam - 1) == ' ' || *(nam - 1) == '\t')
  {
    nam--;
  }
  *nam = '\0';

  // printf("INPUT to variable named '%s'\n", name);
  printf("? ");
  char userinput[255] = "";
  fgets(userinput, 255, stdin);

  // remove last newline
  userinput[strlen(userinput) - 1] = '\0';

  variable_write(name, userinput);
}

char escapeChar(char c)
{
  switch (c)
  {
  case 'b':
    return '\b';
  case 'f':
    return '\f';
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 't':
    return '\t';
  case '\\':
    return '\\';
  case '\'':
    return '\'';
  case '\"':
    return '\"';
  default:
    return c;
  }
}