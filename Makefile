CC=gcc
CFLAGS=-Wall

basic: basic.c keywords.c variables.c tokenizer.c lines.c
	$(CC) $(CFLAGS) -o basic keywords.c variables.c tokenizer.c lines.c basic.c