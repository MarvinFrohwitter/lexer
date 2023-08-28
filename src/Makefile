# The lexer that tokenise the given C code.

SRC = lexer.c
OBJ = ${SRC:.c=.o}

CC = cc
LIBS =
CFLAGS = -O3 -pedantic -Wall -Wextra -std=c11 -ggdb

all: options lexer

options:
	@echo lexer build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CC     = ${CC}"

lexer:
	${CC} -o $@ ${SRC} ${CFLAGS} ${LIBS}

clean:
	rm -f lexer

.PHONY: all clean options lexer
