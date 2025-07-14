#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "variables.h"

void kw_let(char *parm)
{
  printf("LET %s\n", parm);

  char *name;
  char *value;

  TokenList *tokenlist = tokenize(parm);
  // get first token that isn't a whitespace
  // TODO: Maybe get rid of the tokenlist, and just call nextToken here - expand with nextNonWhitespaceToken - and the possibility of an EndToken of sorts.
  int i=0;
  Token* token;
  do
  {
    token = tokenlist->tokens[i++];
  } while (token->type == WHITESPACE);
  
  // Token MUST be an identifier
  if(token->type != IDENTIFIER)
  {
    // TODO: Better error handling
    printf("SYNTAX ERROR - LET must be followed by identifier\n");
    return;
  }
  name = token->value;
  
  // skip whitespace
  do
  {
    token = tokenlist->tokens[i++];
  } while (token->type == WHITESPACE);
  
  // Token MUST be equal
  if(token->type != EQUALS)
  {
    // TODO: Better error handling
    printf("SYNTAX ERROR - missing equals after identifier\n");
    return;
  }
  
  // skip whitespace
  do
  {
    token = tokenlist->tokens[i++];
  } while (token->type == WHITESPACE);

  // Last token can be either identifier or string - but don't be too strict about it
  if(token->type == STRING || token->type == IDENTIFIER)
  {
    value = token->value;
  } else 
  {
    // TODO: Better error handling
    printf("ERROR? Don't know how to handle token type %s(%s)\n", TOKEN_NAMES[token->type], token->value);
  }

  // store variable name and value
  variable_create(name, value);
  variables_dump();
}

void kw_print(char *parm)
{
  //  printf("PRINT %s\n", parm);

  TokenList *tokenlist = tokenize(parm);
  for (int i = 0; i < tokenlist->length; i++)
  {
    Token *token = tokenlist->tokens[i];
    if (token->type == WHITESPACE)
    {
      // just ignore it
    }
    else if (token->type == STRING)
    {
      // print value
      printf("%s", token->value);
    }
    else if (token->type == IDENTIFIER)
    {
      // find variable with that name - read it and print the value
      printf("%s", variable_read(token->value));
    }
  }
}

void kw_input(char *parm)
{
  printf("INPUT %s\n", parm);

  TokenList *tokenlist = tokenize(parm);
  // get first token that isn't a whitespace - just like in kw_let
  int i=0;
  Token* token;
  do
  {
    token = tokenlist->tokens[i++];
  } while (token->type == WHITESPACE);

  // if the token is a string, use it as a prompt
  if(token->type == STRING)
  {
    printf("%s", token->value);
    // and skip whitespace
    do
    {
      token = tokenlist->tokens[i++];
    } while (token->type == WHITESPACE);
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