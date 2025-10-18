#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "variables.h"
#include "keywords.h"
#include "lines.h"

void assign_variable(Token *token, char *parm)
{
  // Find variable with the name of this token
  Variable *v = getVariable(token->value);
  if (v == NULL)
  {
    printf("ERROR - vairable '%s' does not exist!\n", token->value);
  }
  else
  {
    // assume that the next token is an equals
    token = nextTokenIgnoreWhitespace(&parm);

    if (token->type != EQUALS)
    {
      printf("SYNTAX ERROR - missing equals after identifier\n");
      return;
    }

    // next token is the value being set
    token = nextTokenIgnoreWhitespace(&parm);

    char *variable_value = token->value;
    if (token->type == STRING)
    {
      writeStringVariable(v->name, variable_value);
    }
    else if (token->type == NUMBER)
    {
      writeIntVariable(v->name, calculateValue(token, &parm));
    }
    else if (token->type == IDENTIFIER)
    {
      if (v->type == VAR_TYPE_INTEGER)
      {
        writeIntVariable(v->name, calculateValue(token, &parm));
      }
      else if (v->type == VAR_TYPE_STRING)
      {
        Variable *v2 = getVariable(token->value);
        writeStringVariable(v->name, v2->stringValue);
      }
    }
  }
  variables_dump();
}

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
    createIntVariable(variable_name, calculateValue(token, &parm));
  }
  else if (token->type == IDENTIFIER)
  {
    // find the variable with this name
    Variable *v = getVariable(variable_value);
    // if variable is an int - calculate the value
    if (v->type == VAR_TYPE_INTEGER)
    {
      createIntVariable(variable_name, calculateValue(token, &parm));
    }
    else
    {
      // create a new of same type, with same value, but the name provided earlier
      createVariable(*v, variable_name);
    }
  }

  variables_dump();
}

extern Line *first_line;

void kw_list(char *parm)
{
  printf("\n");
  // ignore parameters, just list the program - if it exists
  Line *current = first_line;
  while (current != NULL)
  {
    printf("%d %s\n", current->line_number, current->line_text);
    current = current->next;
  }
}

int calculateValue(Token *token, char **text_ptr)
{
  int value = intValueOfToken(token);

  Token *operation = peekNextTokenIgnoreWhitespace(text_ptr);

  while (operation->type == PLUS || operation->type == MINUS || operation->type == MULTIPLY || operation->type == DIVIDE)
  {
    // actually consume token
    operation = nextTokenIgnoreWhitespace(text_ptr);

    // an operation follows - find the token following that
    token = nextTokenIgnoreWhitespace(text_ptr);
    if (operation->type == PLUS)
    {
      value += intValueOfToken(token);
    }
    else if (operation->type == MINUS)
    {
      value -= intValueOfToken(token);
    }
    else if (operation->type == MULTIPLY)
    {
      value *= intValueOfToken(token);
    }
    else if (operation->type == DIVIDE)
    {
      value /= intValueOfToken(token);
    }
    // peek at next token
    operation = peekNextTokenIgnoreWhitespace(text_ptr);
  }
  return value;
}

int intValueOfToken(Token *token)
{
  if (token->type == NUMBER)
  {
    return parseNumber(token->value);
  }
  else if (token->type == IDENTIFIER)
  {
    Variable *v = getVariable(token->value);
    return v->intValue;
  }
  // default to 0
  return 0;
}

void kw_print(char *parm)
{
  //  printf("PRINT %s\n", parm);

  // Create string to print
  char *string = malloc(255);
  *string = '\0';

  Token *token = nextToken(&parm);
  while (token->type != END)
  {
    if (token->type == STRING)
    {
      // strcat(string, token->value);
      sprintf(string, "%s%s", string, token->value);
    }
    else if (token->type == NUMBER)
    {
      sprintf(string, "%s%d", string, calculateValue(token, &parm));
    }
    else if (token->type == IDENTIFIER)
    {
      // find variable with that name
      Variable *v = getVariable(token->value); // TODO: What if variable doesn't exist?
      if (v == NULL)
      {
        printf("ERROR - vairable '%s' does not exist!\n", token->value);
      }
      else
      {
        //- read it and print the value
        switch (v->type)
        {
        case VAR_TYPE_STRING:
          sprintf(string, "%s%s", string, v->stringValue);
          break;
        case VAR_TYPE_INTEGER:
          sprintf(string, "%s%d", string, calculateValue(token, &parm));
          break;
        case VAR_TYPE_FLOAT:
          sprintf(string, "%s%f", string, v->floatValue);
          break;
        }
      }
    }
    // other tokens are just ignored for now - meaning not printed at all

    token = nextToken(&parm);
  }

  // finally, print the string
  printf("\n%s", string); // NOTE: newline for test only - change later
  // free string buffer again
  free(string);
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