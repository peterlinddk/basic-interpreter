#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "variables.h"

void kw_let(char *parm)
{
  printf("LET %s\n", parm);

  char *name;
  char *value;

  // get first token that isn't a whitespace
  Token *token = nextTokenIgnoreWhitespace(&parm);

  // Token MUST be an identifier
  if (token->type != IDENTIFIER)
  {
    // TODO: Better error handling
    printf("SYNTAX ERROR - LET must be followed by identifier\n");
    return;
  }

  name = token->value;

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

  // Last token can be either string or number - don't be too strict about it yet
  if (token->type == STRING || token->type == NUMBER)
  {
    value = token->value;
  }
  else if (token->type == IDENTIFIER)
  {
    value = variable_read(token->value);
  }
  else
  {
    // TODO: Better error handling
    printf("ERROR? Don't know how to handle token type %s(%s)\n", TOKEN_NAMES[token->type], token->value);
    return;
  }

  // store variable name and value
  variable_create(name, value);
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
      // find variable with that name - read it and print the value
      printf("%s", variable_read(token->value));
    }
    // other tokens are just ignored for now - meaning not printed at all

    token = nextToken(&parm);
  }
}

void kw_input(char *parm)
{
  printf("INPUT %s\n", parm);

  // get first token that isn't a whitespace
  Token* token = nextTokenIgnoreWhitespace(&parm);

  // if the token is a string, use it as a prompt
  if(token->type == STRING)
  {
    printf("%s", token->value);
    // and skip whitespace
    token = nextTokenIgnoreWhitespace(&parm);
  }

  // expect the token to be an identifier
  if(token->type != IDENTIFIER)
  {
    printf("SYNTAX ERROR - expected identifier for INPUT");
    return;
  }

  // use identifier as variable name
  printf("? ");
  char userinput[255] = "";
  fgets(userinput, 255, stdin);

  // remove last newline
  userinput[strlen(userinput) - 1] = '\0';

  variable_write(token->value, userinput);
}