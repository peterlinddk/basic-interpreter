#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lines.h"
#include "keywords.h"

Line *first_line = NULL;

void initialize_program()
{
  first_line = NULL;
}

void create_line(Token *token, char *inp)
{
  int line_number = parseNumber(token->value);
  char *line_text = inp;

  // create new Line struct
  Line *line = malloc(sizeof(Line));
  line->line_number = line_number;
  line->line_text = strdup(inp);
  line->next = NULL;

  // if the program is empty, this is the first line
  if (first_line == NULL)
  {
    first_line = line;
  }
  else
  {
    // find the last line
    Line *lastline = first_line;
    while (lastline->next != NULL)
    {
      lastline = lastline->next;
    }
    // and make that point to this line
    lastline->next = line;
  }

  printf("Created a new line %d\n", line_number);
}
