#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFINE_TOKEN_NAMES
#include "tokenizer.h"
#include "tokens.h"

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

Token *peekNextTokenIgnoreWhitespace(char **text_ptr)
{
  Token *token;

  // create a new pointer to the same text
  char *text = *text_ptr;

  // skip whitespace
  do
  {
    token = nextToken(&text);
  } while (token->type == WHITESPACE);

  // if the token is not a whitespace, return it
  return token;
}

Token *nextTokenIgnoreWhitespace(char **text_ptr)
{
  Token *token;

  // skip whitespace
  do
  {
    token = nextToken(text_ptr);
  } while (token->type == WHITESPACE);

  // if the token is not a whitespace, return it
  return token;
}

Token *nextToken(char **text_ptr)
{
  Token *token;

  // check next character, and decide what kind of token it is
  char c = **text_ptr;
  if (c == '\0') // end of text
  {
    token = &TOKEN_end; // use global end token
  }
  else if (isspace(c)) // whitespace
  {
    token = tokenize_whitespace(text_ptr);
  }
  else if (isdigit(c)) // number
  {
    token = tokenize_number(text_ptr);
  }
  else if (c == '\"') // string
  {
    token = tokenize_string(text_ptr);
  }
  else if (c == '=') // equal
  {
    // use global equal token
    token = &TOKEN_equal;
    // and advance text_ptr
    (*text_ptr)++;
  }
  else if (c == '+')
  {
    token = &TOKEN_plus;
    (*text_ptr)++;
  }
  else if (c == '-')
  {
    token = &TOKEN_minus;
    (*text_ptr)++;
  }
  else if (c == '*')
  {
    token = &TOKEN_multiply;
    (*text_ptr)++;
  }
  else if (c == '/')
  {
    token = &TOKEN_divide;
    (*text_ptr)++;
  }
  else // identifier
  {
    token = tokenize_identifier(text_ptr);
  }

  printToken(token);

  return token;
}

void printToken(Token *token)
{
  printf("<%s '%s'> ", TOKEN_NAMES[token->type], token->value);
}

Token *tokenize_whitespace(char **text_ptr)
{
  // create new token
  Token *token = malloc(sizeof(Token));
  token->type = WHITESPACE;

  char string[255] = "";
  char *val = string;
  while (isspace(**text_ptr))
  {
    *val++ = *(*text_ptr)++;
  }
  *val = '\0';
  token->value = strdup(string);

  return token;
}

Token *tokenize_number(char **text_ptr)
{
  // create new token of type number
  Token *token = malloc(sizeof(Token));
  token->type = NUMBER;

  char string[255] = "";
  char *val = string;
  while (isdigit(**text_ptr))
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
  while (**text_ptr != '\0' && **text_ptr != '"')
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
  // TODO: Maybe give an error or warning if string wasn't terminated with " ...
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
  // An identifier can end in several ways:
  // - with the end of the text, or if it meets any of these characters: = " + - * / or a space
  // - if a digit follows a space, the identifier ended before the digit!
  int stopped = 0;
  do
  {
    char c = **text_ptr;
    if (c == '\0')
      stopped = 1;
    if (c == '=' || c == '\"' || c == '+' || c == '-' || c == '*' || c == '/' || c == ' ')
      stopped = 1;
    if (c == ' ' && isdigit(*(*text_ptr + 1)))
      stopped = 1;

    if (!stopped)
      *val++ = *(*text_ptr)++;
  } while (!stopped);

  // remove trailing whitespace - if any
  while (isspace(*(val - 1)))
  {
    val--;
  }
  // make sure to terminate the string
  *val = '\0';

  // check if identifier is keyword
  token->type = matchKeywordToken(string);
  token->value = strdup(string);

  return token;
}

/* returns either IDENTIFIER, or KEYWORD TOKEN*/
TOKEN_TYPE matchKeywordToken(char *identifier)
{
  if (strncasecmp(identifier, "INPUT", 6) == 0)
  {
    return KW_INPUT;
  }
  else if (strncasecmp(identifier, "LET", 4) == 0)
  {
    return KW_LET;
  }
  else if (strncasecmp(identifier, "PRINT", 6) == 0)
  {
    return KW_PRINT;
  }

  return IDENTIFIER;
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