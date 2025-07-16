#ifndef KEYWORDS_H
#define KEYWORDS_H

static const char *keywords[] = {"NEW", "LIST", "RUN", "END", "REM", "LET", "PRINT", "INPUT"};

void kw_let(char *parameter);
void kw_print(char *parameter);
void kw_input(char *parameter);

int parseNumber(char *str);

#endif