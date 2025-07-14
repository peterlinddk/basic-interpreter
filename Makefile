CC=gcc
CFLAGS=-Wall

basic: basic.c keywords.c variables.c tokenizer.c
	$(CC) $(CFLAGS) -o basic keywords.c variables.c tokenizer.c basic.c