#include <stdio.h>
#include <string.h>
#include <ctype.h>

int startsWithIgnoreCase(char *string, const char *word);
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

  // find start and end of string
  char* start = parm;
  while(*start != '"') start++;
  start++;
  char* end = start;
  while(*end != '"') end++;

  // print string between start and end
  *end = '\0';
  printf("String: %s", start);
/*  while(start < end)
  {
    putchar(*start);
    start++;
  }
    */
}

void kw_input(char *parm)
{
  printf("INPUT %s\n", parm);
}