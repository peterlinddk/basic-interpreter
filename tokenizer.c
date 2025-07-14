#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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