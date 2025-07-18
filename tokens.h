#ifndef TOKENS_H
#define TOKENS_H

#include "tokenizer.h"

Token TOKEN_equal = {.type = EQUALS, .value = "="};
Token TOKEN_plus = {.type = PLUS, .value = "+"};
Token TOKEN_minus = {.type = MINUS, .value = "-"};
Token TOKEN_end = {.type = END, .value = "-end of tokens-"};

#endif