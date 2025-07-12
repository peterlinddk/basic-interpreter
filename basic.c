#include <stdio.h>
#include <string.h>
#include <ctype.h>

int startsWithIgnoreCase(char *string, const char *word);
char escapeChar(char c);

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

    printf("You entered '%s'\n", inputline);
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

void kw_let(char *parm)
{
  printf("LET %s\n", parm);
}

void kw_print(char *parm)
{
  // ignore whitespace in parameter
  while (*parm == ' ' || *parm == '\t')
    parm++;

  printf("PRINT %s\n", parm);

  // create a new string to print
  char string[255] = "";

  // find start of string in input
  char *input = parm;
  while (*input++ != '"');  
  input++; // ignore the first "

  // add each character to the string
  char *str = string;
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

  // print string
  printf("%s", string);
}

void kw_input(char *parm)
{
  printf("INPUT %s\n", parm);
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