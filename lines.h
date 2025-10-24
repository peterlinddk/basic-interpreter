#ifndef LINES_H
#define LINES_H

#include "tokenizer.h"

typedef struct Line Line;

struct Line {
  int line_number;
  char *line_text;
  Line *next;
};

void create_line(Token *token, char* inp);
void delete_line(Token *token);

#endif