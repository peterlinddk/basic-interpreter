#include <stdio.h>
#include <string.h>

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

    printf("You entered '%s'\n", inputline);
  } while (strncmp(inputline, "QUIT", 4) != 0);

  return 0;
}