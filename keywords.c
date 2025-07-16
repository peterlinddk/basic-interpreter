#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "variables.h"
#include "keywords.h"

void kw_let(char *parm)
{
  printf("LET %s\n", parm);

  // get first token that isn't a whitespace
  Token *token = nextTokenIgnoreWhitespace(&parm);

  // Token MUST be an identifier
  if (token->type != IDENTIFIER)
  {
    // TODO: Better error handling
    printf("SYNTAX ERROR - LET must be followed by identifier\n");
    return;
  }

  char *variable_name = token->value;

  // get next token that isn't a whitespace
  token = nextTokenIgnoreWhitespace(&parm);

  // Token MUST be equal
  if (token->type != EQUALS)
  {
    // TODO: Better error handling
    printf("SYNTAX ERROR - missing equals after identifier\n");
    return;
  }

  // get next token that isn't a whitespace
  token = nextTokenIgnoreWhitespace(&parm);

  char *variable_value = token->value;

  // Last token can be either string or number - or another identifier
  if (token->type == STRING)
  {
    createStringVariable(variable_name, variable_value);
  }
  else if (token->type == NUMBER)
  {
    createIntVariable(variable_name, parseNumber(variable_value));
  }
  else if (token->type == IDENTIFIER)
  {
    // find the variable with this name
    Variable *v = getVariable(variable_value);
    // and create a new of same type, with same value, but the name provided earlier
    createVariable(*v, variable_name);
  }
  else
  {
    // TODO: Better error handling
    printf("ERROR? Don't know how to handle token type %s(%s)\n", TOKEN_NAMES[token->type], token->value);
    return;
  }

  variables_dump();
}

void kw_print(char *parm)
{
  //  printf("PRINT %s\n", parm);

  Token *token = nextToken(&parm);
  while (token->type != END)
  {
    if (token->type == STRING)
    {
      // print value
      printf("%s", token->value);
    }
    else if (token->type == NUMBER)
    {
      // print number value
      printf("%s", token->value);
    }
    else if (token->type == IDENTIFIER)
    {
      // find variable with that name
      Variable *v = getVariable(token->value); // TODO: What if variable doesn't exist?
      //- read it and print the value
      switch (v->type)
      {
      case VAR_TYPE_STRING:
        printf("%s", v->stringValue);
        break;
      case VAR_TYPE_INTEGER:
        printf("%d", v->intValue);
        break;
      case VAR_TYPE_FLOAT:
        printf("%f", v->floatValue);
        break;
      }
    }
    // other tokens are just ignored for now - meaning not printed at all

    token = nextToken(&parm);
  }
}

void kw_input(char *parm)
{
  printf("INPUT %s\n", parm);

  // get first token that isn't a whitespace
  Token *token = nextTokenIgnoreWhitespace(&parm);

  // if the token is a string, use it as a prompt
  if (token->type == STRING)
  {
    printf("%s", token->value);
    // and skip whitespace
    token = nextTokenIgnoreWhitespace(&parm);
  }

  // expect the token to be an identifier
  if (token->type != IDENTIFIER)
  {
    printf("SYNTAX ERROR - expected identifier for INPUT");
    return;
  }
  char *variable_name = token->value;

  // get input as string
  printf("? ");
  char userinput[255] = "";
  fgets(userinput, 255, stdin);
  // remove last newline
  userinput[strlen(userinput) - 1] = '\0';

  // check variable-type
  Variable *v = getVariable(variable_name);

  if (v->type == VAR_TYPE_STRING)
  {
    writeStringVariable(variable_name, userinput);
  }
  else if (v->type == VAR_TYPE_INTEGER)
  {
    // don't trust the user, so tokenize number first
    char *input_ptr = userinput;
    Token *input_value = nextTokenIgnoreWhitespace(&input_ptr);
    if (input_value->type != NUMBER)
    {
      printf("TYPE MISMATCH ERROR - expected number, got '%s'\n", input_value->value);
      return;
    }
    writeIntVariable(variable_name, parseNumber(input_value->value));
  }
  // TODO: Handle float

  variables_dump();
}

int parseNumber(char *str)
{
  int value = 0;
  while (*str)
  {
    value *= 10; // base 10 is default
    value += (*str++) - '0';
  }
  return value;
}