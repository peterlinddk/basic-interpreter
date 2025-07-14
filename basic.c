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

typedef enum
{
  WHITESPACE,
  STRING,
  IDENTIFIER,
  EQUALS
} TOKEN_TYPE;

typedef struct
{
  TOKEN_TYPE type;
  char *value;
} Token;

typedef struct
{
  Token **tokens;
  int length;
  int capacity;

} TokenList;

TokenList *tokenize(char *line);
void printToken(Token *token);

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

  tokenize(parm);

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
  tokenize(parm);

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

  tokenize(parm);

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

// ** Tokens **

const char *TOKEN_NAMES[] = {
    "WHITESPACE",
    "STRING",
    "IDENTIFIER",
    "EQUALS"};

Token TOKEN_equal = {.type = EQUALS, .value = "="};

Token *nextToken(char **line);
Token *tokenize_whitespace(char **text);
Token *tokenize_string(char **text);
Token *tokenize_identifier(char **text);
void printToken(Token *token);

TokenList *tokenize(char *text)
{
  //  printf("TOKENIZE - begin\n================\n");

  // create list of tokens with capacity of 5
  Token **tokens = malloc(sizeof(Token) * 5);
  TokenList *tokenlist = malloc(sizeof(TokenList));
  tokenlist->capacity = 5;
  tokenlist->length = 0;
  tokenlist->tokens = tokens;

  while (*text != '\0')
  {
    // check if the list's run out of room
    if (tokenlist->length >= tokenlist->capacity)
    {
      // then double the capacity
      tokenlist->capacity *= 2;
      // and reallocate
      tokenlist->tokens = realloc(tokenlist->tokens, tokenlist->capacity * sizeof(Token));
    }
    tokenlist->tokens[tokenlist->length++] = nextToken(&text);
  }

  // Print the contents of the tokenlist - for testing
  /*
  printf("TOKENS in list:\n");
  for(int i=0; i < tokenlist->length; i++) {
    printToken(tokenlist->tokens[i]);
  }
  */

  //  printf("TOKENIZE - end\n==============\n");
  return tokenlist;
}

Token *nextToken(char **text_ptr)
{
  Token *token;

  // check next character, and decide what kind of token it is
  char c = **text_ptr;
  if (c == ' ' || c == '\t')  // whitespace
  {
    token = tokenize_whitespace(text_ptr);
  }
  else if (c == '\"')         // string
  {
    token = tokenize_string(text_ptr);
  }
  else if (c == '=')          // equal
  {
    // use global equal token
    token = &TOKEN_equal;
    // and advance text_ptr
    (*text_ptr)++;
  }
  else                        // identifier
  {
    token = tokenize_identifier(text_ptr);
  }

  return token;
}

void printToken(Token *token)
{
  printf("Token:\n type: %s\n value: '%s'\n\n", TOKEN_NAMES[token->type], token->value);
}

Token *tokenize_whitespace(char **text_ptr)
{
  // create new token
  Token *token = malloc(sizeof(Token));
  token->type = WHITESPACE;

  char string[255] = "";
  char *val = string;
  while (**text_ptr == ' ' || **text_ptr == '\t')
  {
    *val++ = *(*text_ptr)++;
  }
  *val = '\0';
  token->value = strdup(string);

  return token;
}

Token *tokenize_string(char **text_ptr)
{
  Token *token = malloc(sizeof(Token));
  token->type = STRING;

  char string[255] = "";
  char *val = string;

  (*text_ptr)++;
  // add each character to the string
  while (**text_ptr != '"')
  {
    if (**text_ptr != '\\')
    {
      *val++ = *(*text_ptr)++;
    }
    else
    {
      // character was \ - read next character
      (*text_ptr)++;
      *val++ = escapeChar(*(*text_ptr)++);
    }
  }
  // make sure to terminate the string
  *val = '\0';
  // and scroll past the last "
  (*text_ptr)++;

  token->value = strdup(string);

  return token;
}

Token *tokenize_identifier(char **text_ptr)
{
  Token *token = malloc(sizeof(Token));
  token->type = IDENTIFIER;

  char string[255] = "";
  char *val = string;
  // end with end of text, or a = or a " ... will probably change later!!!
  while (**text_ptr != '\0' && **text_ptr != '=' && **text_ptr != '\"')
  {
    *val++ = *(*text_ptr)++;
  }
  // remove trailing whitespace - if any
  while (*(val - 1) == ' ' || *(val - 1) == '\t')
  {
    val--;
  }
  // make sure to terminate the string
  *val = '\0';
  token->value = strdup(string);

  return token;
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