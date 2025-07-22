#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "tokenizer.h"

void assign_variable(Token *token, char *parm);
void kw_let(char *parameter);
void kw_print(char *parameter);
void kw_input(char *parameter);

int calculateValue(Token *token, char **text_ptr);
int intValueOfToken(Token *token);
int parseNumber(char *str);


#endif