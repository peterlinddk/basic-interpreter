#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum
{
  WHITESPACE,
  NUMBER,
  STRING,
  IDENTIFIER,
  EQUALS,
  PLUS,
  MINUS,
  END
} TOKEN_TYPE;

#pragma GCC diagnostic ignored "-Wunused-variable"
static const char *TOKEN_NAMES[] = {
    "WHITESPACE",
    "NUMBER",
    "STRING",
    "IDENTIFIER",
    "EQUALS",
    "PLUS",
    "MINUS",
    "END"};

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
Token *nextToken(char **line);
Token *nextTokenIgnoreWhitespace(char **text_ptr);
Token *tokenize_whitespace(char **text);
Token *tokenize_number(char **text_ptr);
Token *tokenize_string(char **text);
Token *tokenize_identifier(char **text);

char escapeChar(char c);

#endif