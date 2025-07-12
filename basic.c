#include <stdio.h>
#include <string.h>
#include <ctype.h>

int startsWithIgnoreCase(char *string, const char *word);

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