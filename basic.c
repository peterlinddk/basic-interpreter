#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "keywords.h"
#include "variables.h"
#include "tokenizer.h"
#include "lines.h"

int startsWithIgnoreCase(char *string, const char *word);
int isEmpty(char *string);

int main()
{
  printf("Welcome to BASIC\n");
  char inputline[255] = "";

  do
  {
    printf("\nready.\n");
    printf(">");

    fgets(inputline, 255, stdin);
    // remove last newline
    inputline[strlen(inputline) - 1] = '\0';

    // Tokenize input
    char *inp = inputline;
    Token *token = nextTokenIgnoreWhitespace(&inp);

    switch(token->type)
    {
      case NUMBER: // an empty input after the number, means delete a line
      if (isEmpty(inp))
      {
        delete_line(token);
      }
      else      // some input after the number, means create a new line
      {
        create_line(token, inp);
      }
      break;
      case IDENTIFIER: assign_variable(token, inp); break;
      case KW_INPUT: kw_input(inp); break;
      case KW_LET: kw_let(inp); break;
      case KW_LIST: kw_list(inp); break;
      case KW_PRINT: kw_print(inp); break;
      default:
        printf("Unknown token type '%s' at beginning of line: '%s'\n", TOKEN_NAMES[token->type], inputline); 
    }

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

int isEmpty(char *s)
{
  do
  {
    if (!(*s == '\0' || *s == ' ' || *s == '\t' || *s == '\n' || *s == '\r'))
    {
      return 0;
    }
  } while (*++s != 0);

  return 1;
}