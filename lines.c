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

void delete_line(Token *token)
{
  int line_number = parseNumber(token->value);
  printf("Delete line %d", line_number);

  Line *current_line = first_line;
  // remember the previous line looked at
  Line *previous_line = NULL;

  while (current_line != NULL)
  {
    // if the new line number matches - delete it
    if (line_number == current_line->line_number)
    {
      Line *deleted_line = current_line;
      // make the previous line point to the next line
      // if it was NULL, the first line is replaced
      if(previous_line == NULL) 
      {
        first_line = deleted_line->next;
      }
      else 
      {
        previous_line->next = deleted_line->next;
      }

      // finally, free the old replaced line and its' text
      free(deleted_line->line_text);
      free(deleted_line);

      // and we are done - end the loop
      break;
    }

    // remember the previous line
    previous_line = current_line;
    // and go on to the next one
    current_line = current_line->next;
  }
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
    // the new line is either:
    // - added after the last line | if line_number is greater than any line
    // - replacing an existing line | if line_number exists
    // - inserted before an existing line | if line_number is less than ...

    // look at the current line
    Line *current_line = first_line;
    // remember the previous line looked at
    Line *previous_line = NULL;

    while (current_line != NULL)
    {
      // if the new line number matches - replace it
      if (line->line_number == current_line->line_number)
      {
        Line *replaced_line = current_line;
        // make the previous line point to this line
        // if it was NULL, the first line is replaced
        if (previous_line == NULL)
        {
          first_line = line;
        }
        else
        {
          previous_line->next = line;
        }
        // and point onwards to the next line
        line->next = replaced_line->next;
        // finally, free the old replaced line and its' text
        free(replaced_line->line_text);
        free(replaced_line);

        // and we are done - end the loop
        break;
      }
      // if this was the last line - and the line_number is greater, add this one to the end

      // if the new line is before the current one - insert it before
      else if (line->line_number < current_line->line_number)
      {
        // make the previous line point to this
        // if it was NULL, the first line is 'inserted'
        if (previous_line == NULL)
        {
          first_line = line;
        }
        else
        {
          previous_line->next = line;
        }
        // and make this point on to the current line
        line->next = current_line;

        // and we are done - end the loop
        break;
      }
      else if (current_line->next == NULL) // line->line_number > current_line->line_number &&
      {
        current_line->next = line;

        // and we are done - end the loop
        break;
      }

      // remember the previous line
      previous_line = current_line;
      // and go on to the next one
      current_line = current_line->next;
    };
  }

  printf("Created a new line %d\n", line_number);
}